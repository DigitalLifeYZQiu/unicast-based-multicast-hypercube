//
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <iomanip>
#include "common.h"
#include "Hypercube.h"
#include "RoutingHypercube.h"
#include "Event.h"

// 全局统计变量
int messarrive = 0;               // 成功到达目的地的消息计数
double total_latency = 0;         // 所有消息延迟总和（周期数）
int allmess = 0;                  // 总生成消息计数
int total_flits_delivered = 0;    // 成功传递的总flit数
std::map<Message*, int> messageGenTime; // 记录消息生成时间

// 运行单次模拟的函数
void runSimulation(double injection_rate, std::ofstream& resultsFile) {
    // 重置统计变量
    messarrive = 0;
    total_latency = 0;
    allmess = 0;
    total_flits_delivered = 0;
    messageGenTime.clear();

    int bufferSize = 10; // 每个缓冲区大小（以flit为单位）
    int dimension=4;
    // 创建超立方体网络
    Hypercube cube(dimension, bufferSize * MESSLENGTH);

    // 创建路由算法
    RoutingHypercube routing(&cube);

    // 创建事件处理器
    Event event(&routing);

    // 消息队列
    std::vector<Message*> messages;

    // 主循环
    for (int cycle = 0; cycle < totalcircle; ++cycle) {
        // 根据注入率生成新消息
        // double gen_prob = injection_rate / MESSLENGTH;  // 将flit注入率转换为消息生成概率
        double gen_prob = injection_rate;
        if ((double)rand() / RAND_MAX < gen_prob) {
            Message* newMsg = event.generateMessage();
            if (newMsg) {
                messages.push_back(newMsg);
                messageGenTime[newMsg] = cycle;  // 记录生成时间
                allmess++;
            }
        }

        // 处理消息
        for (auto it = messages.begin(); it != messages.end(); ) {
            Message* msg = *it;
            event.forwardMessage(*msg);

            if (!msg->active) {  // 消息已到达目的地
                // 计算并记录延迟
                int genTime = messageGenTime[msg];
                int latency = cycle - genTime;
                total_latency += latency;
                messarrive++;

                // 记录传递的flit数
                total_flits_delivered += MESSLENGTH;

                // 从消息列表中移除
                delete msg;
                it = messages.erase(it);
            } else {
                ++it;
            }
        }
    }

    // 计算性能指标
    double avg_latency = (messarrive > 0) ? (total_latency / messarrive) : 0;////s->totalcir/s->messarrive 平均延迟的计算公式

    double throughput = injection_rate * (float) messarrive / allmess;//linkrate * ((float) s->messarrive / allmess)吞吐量的计算公式
    double traffic = (double)total_flits_delivered / totalcircle / cube.getNodeCount();

    // 输出结果
    std::cout << "Injection Rate: " << std::fixed << std::setprecision(4) << injection_rate
              << " | Avg Latency: " << avg_latency << " | Throughput: " << throughput
              << " | Traffic: " << traffic << " flits/node/cycle"
              << " | Delivered: " << messarrive << "/" << allmess << " messages"
              << std::endl;

    // 写入结果文件
    resultsFile << injection_rate << " " << avg_latency << " " << throughput << "\n";

    // 清理剩余消息
    for (auto msg : messages) {
        delete msg;
    }
}

int main() {
    // 打开结果文件
    std::ofstream resultsFile("results.txt");
    if (!resultsFile) {
        std::cerr << "Error opening results file!" << std::endl;
        return 1;
    }

    resultsFile << "InjectionRate AvgLatency Throughput\n";  // 文件头

    // 测试不同的注入率 (0.01 到 1)
    const int num_rates = 20;
    const double max_rate = 0.3;

    for (int i = 0; i <= num_rates; ++i) {
        double injection_rate = (i * max_rate) / num_rates;
        runSimulation(injection_rate, resultsFile);
    }

    resultsFile.close();
    return 0;
}
