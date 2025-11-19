#!/bin/bash

# ====================================================================
# 【服务器管理脚本】: server.sh
# 作用: 批量或单独启动、停止、重启和查看测试服务器状态。
# 用法: ./server.sh <start|stop|restart|status> <server_name|all>
# 示例: ./server.sh start all # 启动所有服务
# 示例: ./server.sh stop all # 停止指定服务
# ====================================================================

script_path="$(readlink -f "$0")"

script_dir="$(dirname "$script_path")"

root_dir="$(dirname "$script_dir")"

bin_dir="${root_dir}/bin"

action="$1"
target="$2" # server_name 或 all
build_type="$3" # 接收可选的第三个参数

# 检查参数数量（允许 2 或 3 个参数）
if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
    echo "用法: $0 <start|stop|restart|status> <server_name|all> [build_type]"
    echo "注意: build_type 是可选的，默认值：release"
    exit 1
fi

# 设置默认 build_type
if [ -z "$build_type" ]; then
    build_type="release"
fi

server_root_dir="${bin_dir}/${build_type}"

# ----------------------------------------------------
# 【请在此处定义你的服务器列表】
# 格式: "唯一名称:可执行文件名:PID文件名"
# 注意：冒号前是用于命令行的“唯一名称”
# ----------------------------------------------------
declare -A servers=(
    ["registry"]="${server_root_dir}/service_registry_server:${server_root_dir}/service_registry.pid"
    ["auth"]="${server_root_dir}/auth_server:${server_root_dir}/auth.pid"
    ["payment"]="${server_root_dir}/payment_server:${server_root_dir}/payment.pid"
    ["admin"]="${server_root_dir}/admin_server:${server_root_dir}/admin.pid"
    ["chat"]="${server_root_dir}/chat_server:${server_root_dir}/chat.pid"
    ["gateway"]="${server_root_dir}/gateway_server:${server_root_dir}/gateway.pid"
    ["player"]="${server_root_dir}/player_server:${server_root_dir}/player.pid"
    ["logic"]="${server_root_dir}/logic_server:${server_root_dir}/logic.pid"
    ["game"]="${server_root_dir}/game_server:${server_root_dir}/game.pid"
    ["battle"]="${server_root_dir}/battle_server:${server_root_dir}/battle.pid"
    ["cross"]="${server_root_dir}/cross_server:${server_root_dir}/cross.pid"
    ["log"]="${server_root_dir}/log_server:${server_root_dir}/log.pid"
)

# 检查操作是否有效
if [[ ! "$action" =~ ^(start|stop|restart|status)$ ]]; then
    echo "错误: 无效操作 '$action'。请使用 start, stop, restart 或 status。"
    exit 1
fi

# ----------------------------------------------------
# 辅助函数：根据 target 确定要操作的服务器列表
# ----------------------------------------------------
get_target_servers() {
    local -n server_list=$1 # 使用 name 引用外部数组
    
    if [ "$target" = "all" ]; then
        server_list=("${!servers[@]}") # 所有服务器的名称
    else
        # 检查 target 是否在 servers 列表中
        if [[ -v servers["$target"] ]]; then
            server_list=("$target") # 单个服务器的名称
        else
            echo "错误: 未知服务器名称 '$target'。" >&2
            exit 1
        fi
    fi
}

# 辅助函数：解析服务器配置
# 输入: server_name
# 输出: executable pid_file
parse_server_config() {
    local server_name="$1"
    local config="${servers[$server_name]}"
    local IFS=':'
    read -r executable pid_file <<< "$config"
    echo "$executable" "$pid_file"
}

# 函数：检查单个进程状态
check_status() {
    local name="$1"
    local exe="$2"
    local pid_file="$3"
    
    if [ ! -f "$pid_file" ]; then
        echo "🔴 $name ($exe): 未运行 (PID文件不存在)"
        return 1
    fi
    
    local PID=$(cat "$pid_file")
    
    if ps -p "$PID" > /dev/null; then
        echo "🟢 $name ($exe): 运行中 (PID: $PID)"
        return 0
    else
        echo "🟡 $name ($exe): 停止 (PID $PID 不存在)。已清理残留PID文件。"
        rm -f "$pid_file"
        return 1
    fi
}

# 函数：启动服务
start_server() {
    local name="$1"
    local exe="$2"
    local pid_file="$3"

    echo ">>>> 正在启动: $name ($exe) <<<<"

    # 1. 检查是否已运行
    if [ -f "$pid_file" ]; then
        local pid=$(cat "$pid_file")
        if ps -p "$pid" > /dev/null; then
            echo "警告: $name 正在运行 (pid: $pid)。跳过启动。"
            return 0
        else
            echo "警告: pid 文件存在，但进程已停止。清理后重新启动。"
            rm -f "$pid_file"
        fi
    fi
    
    # 2. 启动程序
    local exe_basename=$(basename "$exe")

    (cd "$server_root_dir" && nohup "./$exe_basename" > "$name.log" 2>&1 &)
    
    local max_wait=5
    local current_wait=0
    local success=0

    while [ $current_wait -lt $max_wait ]; do
        if [ -f "$pid_file" ]; then
            success=1
            break
        fi
        sleep 0.5
        current_wait=$((current_wait + 1))
    done

    # 3. 检查 PID 文件是否成功创建
    if [ $success -eq 1 ]; then
        local pid=$(cat "$pid_file")
        echo "✅ 成功启动 $name。pid: $pid"
    else
        echo "❌ 启动失败！$name 未创建 $pid_file。请检查程序路径和权限。"
    fi
}

# 函数：停止服务
stop_server() {
    local name="$1"
    local exe="$2"
    local pid_file="$3"

    echo ">>>> 正在处理: $name ($exe) <<<<"

    if [ ! -f "$pid_file" ]; then
        echo "警告: $pid_file 不存在。$name 可能未运行。"
        return 0
    fi
    
    local pid=$(cat "$pid_file")
    
    if ps -p "$pid" > /dev/null; then
        echo "正在发送 SIGTERM 信号给 PID: $pid..."
        kill "$pid"
        
        sleep 2
        
        if ps -p "$pid" > /dev/null; then
            echo "进程 $pid 未退出，发送 SIGKILL 信号..."
            kill -9 "$pid"
        fi
        
        if [ -f "$pid_file" ]; then
            echo "⚠️ 进程已停止，但残留 PID 文件 $pid_file 已被手动删除。"
            rm -f "$pid_file"
        fi
        
        echo "✅ $name 已停止。"
    else
        echo "警告: pid 文件中记录的进程 $pid 不存在。清理 pid 文件 $pid_file。"
        rm -f "$pid_file"
    fi
}

# 主逻辑开始

# 确定要操作的服务器列表
declare -a target_list
get_target_servers target_list

echo "================================================"
echo "          操作: $action 目标: $target          "
echo "================================================"

# 如果是重启操作，先执行停止
if [ "$action" = "restart" ]; then
    echo "执行停止操作..."
    for name in "${target_list[@]}"; do
        read -r exe pid_file <<< "$(parse_server_config "$name")"
        stop_server "$name" "$exe" "$pid_file"
    done
    echo "等待 3 秒..."
    sleep 3
    action="start" # 将操作切换为启动
fi

# 执行最终操作 (start, stop, status)
for name in "${target_list[@]}"; do
    read -r exe pid_file <<< "$(parse_server_config "$name")"
    
    case "$action" in
        start)
            start_server "$name" "$exe" "$pid_file"
            ;;
        stop)
            stop_server "$name" "$exe" "$pid_file"
            ;;
        status)
            check_status "$name" "$exe" "$pid_file"
            ;;
    esac
done

echo ""
echo "================================================"
echo "          $action 任务完成 ($target)。          "
echo "================================================"