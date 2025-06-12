/**
 * @file main.cpp
 * @brief 超立方体网络模拟器主程序
 * 实现超立方体网络的性能模拟，包括消息生成、路由和性能统计
 */

#include "common.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// 全局变量定义
int ALGORITHM;        // 路由算法选择（当前只实现了一种算法）
int GENERATETYPE;     // 链路速率增长方式选择
int flowalg;          // 流控制算法选择
int totalcircle;      // 程序总运行周期
int dimension;        // 超立方体的维度

// 获取消息队列总大小
int getsize(vector<Message*>* mess);

int main()
{   
	srand(time(NULL));  // 初始化随机数生成器
	
	// 主循环：测试不同的流量模式
	for(int allgen = 1; allgen < 2; allgen++){
		int threshold = 80000;  // 消息数量阈值
		HypercubeRouting* rout1 = NULL;
		GENERATETYPE = allgen;    
		flowalg = 1;
		totalcircle = 100000;  // 总运行周期
		dimension = 8;  // 8维超立方体
		Hypercube* hypercube = NULL;
		Event* s = NULL;
		int r1, r2;
		
		// 输出文件名配置
		string gen[5] = {"0","1", "2", "3", "4"};
		string filename[5] = {"data//gene4//Bubble Flow",
						   "data//gene4//clue-WF",
						   "data//gene4//clue-DOR",
						   "data//gene4//FCclue-DOR",
						   "data//gene4//FCclue-WF",
							 };

		// 根据流量模式修改文件名
		for(int lop=0; lop < 5; lop ++){;
			filename[lop].replace(10, 1, gen[GENERATETYPE]);                 
		}

		// 缓冲区大小配置
		int r1buffer[5] = {1, 2, 1, 2, 2};  // 虚拟通道1缓冲区大小
		int r2buffer[5] = {2, 1, 1, 1, 1};  // 虚拟通道2缓冲区大小
		int alg[5] = {0, 1, 2, 1, 2};       // 路由算法配置
	
		// 路由算法选择（当前只实现了一种算法）
		for(int round = 2 ; round < 3;round++){     
			ofstream out = ofstream(filename[round].c_str());                    
			float linkrate = 0;
			double max = 0;
			
	/************************************************************************************

						start simulate

  ***********************************************************************************/
	//linkrate控制消息产生速率
			for(linkrate = 0.01; linkrate < 1;){

						r1 = r1buffer[round] * MESSLENGTH;    // 转换为flit单位
						r2 = r2buffer[round] * MESSLENGTH;    
				    	hypercube = new Hypercube(dimension, r1, r2);  // 创建超立方体网络
				 

						switch(round){

						case 1: case 2:
						   ALGORITHM = alg[round];                              
						   rout1 = new HypercubeRouting(hypercube);
						   break;
						}
				 
			
					  	s = new Event(rout1);            
				 
								 
			float msgpercir = (float)(linkrate * 2 * dimension/ (MESSLENGTH * 10));
			// float msgpercir = (float)  (linkrate) ;
			//saturationRate = (double)(knode * 2 * 2) / (double)(knode * knode); 在mesh网络中的饱和吞吐量
			//msgpercir = linkrate * saturationRate * knode * knode; 每个周期每个节点产生的flit数
		
			vector<Message*> allvecmess[10];
			float k = 0;
			int allmess = 0;



  	/************************************************************************************

					genarate message

  ***********************************************************************************/
	//执行totalcircle个周期，getsize(allvecmess) < threshold只是自己加的限制条件，可以有也可以删除，具体的threshold和totalcircle值也可以在前面修改									
			for(int i = 0; i < totalcircle && getsize(allvecmess) < threshold; i++){        
				vector<Message*>& vecmess = allvecmess[ i % 10];
				for(  k += msgpercir ; k > 0; k--){                    
							allmess++;  // 总消息数加一
						vecmess.push_back(s->genMes());  // 生成消息并加入队列
				}


	/************************************************************************************

					release link

  ***********************************************************************************/


		for(vector<Message*>::iterator it = vecmess.begin(); it != vecmess.end(); it++){

				/* if the tail of a message shifts , 
				the physical link the message  occupied should release.				
				  */

			if((*it)->releaselink == true){ 
			 	assert((*it)->routpath[MESSLENGTH - 1].buff->linkused);
			 (*it)->routpath[MESSLENGTH - 1].buff->linkused = false;  // 释放链路                    
			 (*it)->releaselink = false;
			}
		}
	



	/************************************************************************************

					forward message				

  ***********************************************************************************/
		for(vector<Message*>::iterator it = vecmess.begin(); it != vecmess.end();){
			if((*it)->active == false){  // 消息到达目的节点
				delete (*it);
				it = vecmess.erase(it);  // 删除已到达的消息                
			}
			else
				s->forwardMes(*(*it++));  // 转发消息                                
		}        
	}




/*****************************************************************************

				output results
  ****************************************************************************/

	int size = getsize(allvecmess);


//s->totalcir/s->messarrive 平均延迟的计算公式；linkrate * ((float) s->messarrive / allmess)吞吐量的计算公式
	cout << endl << endl <<"linkrate:"<< linkrate<<"    arrive:  " << s->messarrive << "    in the network : "
		<< size <<  endl << "average latency: " 
		<< (s->totalcir/s->messarrive) << "  nomalized accepted traffic: "
		<<  linkrate * ((float) s->messarrive / allmess) 
		<<  endl << endl ;
	  
	out << linkrate * ((float) s->messarrive / allmess)  
		<< " " << (s->totalcir/s->messarrive) << endl;





	/************************************************************************************

						whether arrive at saturation point

  ***********************************************************************************/
	if ( linkrate * ((float) s->messarrive / allmess) > max 
		&&  ((linkrate * ((float) s->messarrive / allmess) - max)/ max) > 0.01
		&& getsize(allvecmess) < threshold) 
		max = linkrate * ((float) s->messarrive / allmess);


	else {
			cout << "Saturation point, drain......." << endl;
			drain(allvecmess,hypercube,s);        
			int size = 0;
			for(int j = 0; j < 10; j++){
				if(!allvecmess[j].empty()){
					size += allvecmess[j].size();
				}
			}
			cout << "in the network:      "  << size << endl;
//			outtotest(allvecmess,tor);
//			bufferleft(tor, knode);
//			cout << "max:" << max << endl;
			break;
	}





	/************************************************************************************
                    clean
  *******************************************************************************************/


			for(int m = 0; m < 10; m++){
				for(vector<Message*>::iterator it = allvecmess[m].begin();
							it != allvecmess[m].end(); it++)
					delete (*it);
			}
			delete rout1;
			delete hypercube;
			delete s;    


			switch(GENERATETYPE){
			case 1:                 
					if(linkrate < 0.5) linkrate += 0.05;
					else  linkrate += 0.02;                
					break;

			case 2: case 3: 
					if(linkrate < 0.3) linkrate += 0.1;
					else linkrate += 0.02;
					break;
			case 4:
					if(linkrate < 0.4) linkrate += 0.1;
					else linkrate += 0.02;
					break;


			}
				
		 }   // each linkrate end
	} // round end
  }
  return 1;
}


