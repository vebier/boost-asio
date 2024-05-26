#pragma once
#include "Singleton.h"
#include <queue>
#include <thread>
#include "CSession.h"
#include <queue>
#include <map>
#include <functional>
#include "const.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>


typedef  function<void(shared_ptr<CSession>, short msg_id, string msg_data)> FunCallBack;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	//����Ϣ�ڵ�Ͷ�뵽������
	void PostMsgToQue(shared_ptr <LogicNode> msg);
private:
	LogicSystem();
	//�����������ݾͽ����̴߳���
	void DealMsg();
	//��map����Ϣid�����Ļص���������
	void RegisterCallBacks();
	//����Ϣjsoncpp�����л�Ͷ�ݸ����ͽӿ�
	void HelloWordCallBack(shared_ptr<CSession>, short msg_id, string msg_data);
	std::thread _worker_thread;
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _consume;
	bool _b_stop;
	
	std::map<short, FunCallBack> _fun_callbacks;
};

