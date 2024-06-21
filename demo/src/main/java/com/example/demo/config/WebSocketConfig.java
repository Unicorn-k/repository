package com.example.demo.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.server.standard.ServerEndpointExporter;


@Configuration//标记这个类是一个配置类
@EnableWebSocket//启动WebSocket支持。这意味着Spring会自动配置WebSocket的相关设置。
public class WebSocketConfig {
    @Bean//@Bean注解用于指定一个方法返回的对象应该被Spring管理
    public ServerEndpointExporter serverEndpoint() {
        return new ServerEndpointExporter();
    }
    /*
    用于创建一个ServerEndpointExporter的实例，并将其注册为一个bean。
    ServerEndpointExporter是Spring WebSocket API的一部分，
    它用于自动注册使用了@ServerEndpoint注解的WebSocket endpoint。
    */

}
