package com.example.demo.config;

import lombok.Data;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;

//标记MQTTConfigBuilder类为一个配置类，Spring会把它当作bean的定义来源
//查找配置文件中前缀为publish.mqtt的属性，并将这些属性的值绑定到MQTTConfigBuilder类的相应字段上。
@Configuration
@ConfigurationProperties(MQTTConfigBuilder.PREFIX)
@Data//Lombok为这个类的所有字段生成getter和setter方法，以及equals、canEqual、hashCode、toString方法。
public class MQTTConfigBuilder {

    //配置的名称
    public static final String PREFIX = "publish.mqtt";
    /**
     * 服务端地址
     */
    private String host;//MQTT服务器的主机地址

    /**
     * 客户端id
     */
    private String clientId;//MQTT客户端的ID
    /**
     * 配置链接项
     */
    private MqttConnectOptions options;//MQTT连接的选项

}

