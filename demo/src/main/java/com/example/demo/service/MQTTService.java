package com.example.demo.service;


import com.example.demo.mqtt.MQTTClientUtils;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;


@Service//服务类，spring自动创建实例并存储在spring容器中
public class MQTTService {

    @Resource
    private MQTTClientUtils mqttClientUtils;

    /**
     * 向指定主题发送消息
     * @param topicName
     * @param message
     */
    public void sendMessage(String topicName, String message){
        mqttClientUtils.publish(topicName, message);
    }

    /**
     * 向指定主题发送消息，并指定消息服务质量qos
     * @param topicName 主题名称
     * @param qos qos
     * @param message 具体消息
     */
    public void sendMessage(String topicName,int qos, String message){
        mqttClientUtils.publish(topicName, qos, message);
    }
}

