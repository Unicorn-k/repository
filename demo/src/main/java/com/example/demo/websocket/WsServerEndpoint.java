package com.example.demo.websocket;


import com.example.demo.service.MQTTService;
import com.example.demo.util.JsonDisipose;
import com.example.demo.util.SessionManager;
import com.example.demo.util.SpringApplicationContext;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.websocket.*;
import javax.websocket.server.ServerEndpoint;
import java.io.IOException;
import java.util.Map;


//Java WebSocket API的注解，用于将一个类标记为WebSocket的服务器端点。
// "/MyWebSocket"是这个endpoint的URL路径
@ServerEndpoint("/MyWebSocket")
@Component//Spring的注解，标记这个类是一个组件。Spring会自动创建这个类的实例，并将其存储在Spring容器中。
public class WsServerEndpoint {

    //@Autowired----Spring自动注入实例到变量中
    @Autowired
    private SessionManager sessionManager;
    @Autowired
    private MQTTService mqttService;
    @OnOpen
    public void onOpen(Session session) {//连接成功
        if (sessionManager == null) {
            sessionManager = SpringApplicationContext.getBean(SessionManager.class);
        }
        sessionManager.add(session);
        System.out.println("连接成功，Session ID是：" + session.getId());
        System.out.println("连接成功");
    }
    @OnClose
    public void onClose(Session session) {//连接关闭
        if (sessionManager == null) {
            sessionManager = SpringApplicationContext.getBean(SessionManager.class);
        }
        sessionManager.remove(session);
        System.out.println("连接关闭");
    }

    //下面注释的代码用于app与服务端断开websocket连接时不弹出异常/错误
//    @OnError
//    public void onError(Session session, Throwable throwable) {
//        // 处理错误
//        if (throwable instanceof EOFException) {
//            // 客户端关闭了连接
//            System.out.println("Client closed the connection.");
//        } else {
//            // 其他错误
//            throwable.printStackTrace();
//        }
//    }

    @OnMessage
    public String onMsg(String text) throws IOException {//接收到app的消息
        System.out.println("servet 收到：" + text);

        //app——to——esp
        if (mqttService == null) {
            mqttService = SpringApplicationContext.getBean(MQTTService.class);
        }
        //解析json
        Map<String, Integer> values = JsonDisipose.extractValuesofapp(text);
        int workMode = values.get("WorkMode");
        int massageMode = values.get("MassageMode");
        int currentStrengthLeft = values.get("current_strength_left");
        int currentStrengthRight = values.get("current_strength_right");
        int channelLeft = values.get("channel_left");
        int channelRight = values.get("channel_right");

        // 现使用这些变量
        System.out.println("Work Mode: " + workMode);
        System.out.println("Massage Mode: " + massageMode);
        System.out.println("Current Strength Left: " + currentStrengthLeft);
        System.out.println("Current Strength Right: " + currentStrengthRight);
        System.out.println("Channel Left: " + channelLeft);
        System.out.println("Channel Right: " + channelRight);

        //----重新封装json
        String jsonresult=JsonDisipose.createJsonofapp(
                "iot-2",workMode,massageMode,currentStrengthLeft,currentStrengthRight,channelLeft,channelRight);
        System.out.println("result is"+jsonresult);


        //mqttService.sendMessage("test", text);
        mqttService.sendMessage("test", jsonresult);
        //----------------

        //return "servet 发送：" + text;
        return "ok";
    }
    public void broadcast(String message) {
        sessionManager.getSessions().values().forEach(session -> {
            try {
                session.getBasicRemote().sendText(message);
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
    }

}