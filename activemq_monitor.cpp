#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>

#include <QDebug>
#include <QList>
#include <QString>
#include <QtXml>
#include <iostream>
#include <string>

class queue_struct {
 public:
  QString q_name;
  QString q_size;
  QString q_consumerCount;
  QString q_enqueueCount;
  QString q_dequeueCount;
};

class topic_struct {
 public:
  QString t_name;
  QString t_size;
  QString t_consumerCount;
  QString t_enqueueCount;
  QString t_dequeueCount;
};

std::string JMS_MONITOR_USERNAME = "admin";
std::string JMS_MONITOR_PASSWORD = "admin";
std::string JMS_MONITOR_MQINDEX = "http://localhost:8161";
std::string JMS_MONITOR_MQQUEUE = "/admin/xml/queues.jsp";
std::string JMS_MONITOR_MQTOPIC = "/admin/xml/topics.jsp";

bool C_ACTIVEMQ_QUEUEINFO() {
  RestClient::init();

  RestClient::Connection* conn =
      new RestClient::Connection(JMS_MONITOR_MQINDEX);

  conn->SetBasicAuth(JMS_MONITOR_USERNAME, JMS_MONITOR_PASSWORD);
  conn->SetTimeout(5);

  conn->FollowRedirects(true);

  RestClient::Response r = conn->get(JMS_MONITOR_MQQUEUE);

  if (r.code != 200) {
    std::cout << "Can't login the admin panel" << std::endl;
    std::cout << r.code << std::endl;
    return false;
  }
  // std::cout << r.body << std::endl;

  RestClient::disable();

  QList<queue_struct> queueInfoList;
  QDomDocument xmlBOM;
  if (!xmlBOM.setContent(QString(r.body.c_str()))) {
    return false;
  }
  QDomElement root = xmlBOM.documentElement();

  QDomElement Component = root.firstChild().toElement();

  while (!Component.isNull()) {
    if (Component.tagName() == "queue") {
      QString queueName = Component.attribute("name");
      QDomElement ele = Component.firstChild().toElement();
      QString consumerCount;
      QString size;
      QString enqueueCount;
      QString dequeueCount;

      while (!ele.isNull()) {
        if (ele.tagName() == "stats") {
          size = ele.attribute("size");
          consumerCount = ele.attribute("consumerCount");
          enqueueCount = ele.attribute("enqueueCount");
          dequeueCount = ele.attribute("dequeueCount");
        }
        ele = ele.nextSibling().toElement();
      }

      queue_struct tmp_struct;
      tmp_struct.q_name = queueName;
      tmp_struct.q_size = size;
      tmp_struct.q_consumerCount = consumerCount;
      tmp_struct.q_enqueueCount = enqueueCount;
      tmp_struct.q_dequeueCount = dequeueCount;

      queueInfoList.append(tmp_struct);
    }
    Component = Component.nextSibling().toElement();
  }

  for (int i = 0; i < queueInfoList.size(); i++) {
    queue_struct tmp_struct = queueInfoList.at(i);

    qDebug() << "Queue: " << tmp_struct.q_name << " Size: " << tmp_struct.q_size
             << " consumer count: " << tmp_struct.q_consumerCount
             << " enqueue count: " << tmp_struct.q_enqueueCount
             << " dequeue count: " << tmp_struct.q_dequeueCount;
  }
  return true;
}

bool C_ACTIVEMQ_TOPICINFO() {
  RestClient::init();

  RestClient::Connection* conn =
      new RestClient::Connection(JMS_MONITOR_MQINDEX);

  conn->SetBasicAuth(JMS_MONITOR_USERNAME, JMS_MONITOR_PASSWORD);
  conn->SetTimeout(5);

  conn->FollowRedirects(true);

  RestClient::Response r = conn->get(JMS_MONITOR_MQTOPIC);

  if (r.code != 200) {
    std::cout << "Can't login the admin panel" << std::endl;
    std::cout << r.code << std::endl;
    return false;
  }
  // std::cout << r.body << std::endl;

  RestClient::disable();

  QList<topic_struct> topicInfoList;
  QDomDocument xmlBOM;
  if (!xmlBOM.setContent(QString(r.body.c_str()))) {
    return false;
  }
  QDomElement root = xmlBOM.documentElement();

  QDomElement Component = root.firstChild().toElement();

  while (!Component.isNull()) {
    if (Component.tagName() == "topic") {
      QString topicName = Component.attribute("name");
      QDomElement ele = Component.firstChild().toElement();
      QString consumerCount;
      QString size;
      QString enqueueCount;
      QString dequeueCount;

      while (!ele.isNull()) {
        if (ele.tagName() == "stats") {
          size = ele.attribute("size");
          consumerCount = ele.attribute("consumerCount");
          enqueueCount = ele.attribute("enqueueCount");
          dequeueCount = ele.attribute("dequeueCount");
        }
        ele = ele.nextSibling().toElement();
      }

      topic_struct tmp_struct;
      tmp_struct.t_name = topicName;
      tmp_struct.t_size = size;
      tmp_struct.t_consumerCount = consumerCount;
      tmp_struct.t_enqueueCount = enqueueCount;
      tmp_struct.t_dequeueCount = dequeueCount;

      topicInfoList.append(tmp_struct);
    }
    Component = Component.nextSibling().toElement();
  }

  for (int i = 0; i < topicInfoList.size(); i++) {
    topic_struct tmp_struct = topicInfoList.at(i);

    qDebug() << "Topic: " << tmp_struct.t_name << " Size: " << tmp_struct.t_size
             << " consumer count: " << tmp_struct.t_consumerCount
             << " enqueue count: " << tmp_struct.t_enqueueCount
             << " dequeue count: " << tmp_struct.t_dequeueCount;
  }
  return true;
}

int main() {
  bool is_queue_ok = C_ACTIVEMQ_QUEUEINFO();
  if (!is_queue_ok) {
    qDebug() << "get queue info failed";
  }

  bool is_topic_ok = C_ACTIVEMQ_TOPICINFO();
  if (!is_topic_ok) {
    qDebug() << "get topic info failed";
  }
  return 0;
}
