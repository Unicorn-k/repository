package com.example.demo.util;

import org.springframework.stereotype.Component;

import javax.websocket.Session;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

@Component//这个类是一个组件。Spring会自动创建这个类的实例，并将其存储在Spring容器中。
public class SessionManager {//用于管理WebSocket的会话
    private final Map<String, Session> sessions = new ConcurrentHashMap<>();
    //存储WebSocket的会话。Map的键是会话的ID，值是会话对象。
    public void add(Session session) {
        sessions.put(session.getId(), session);
    }
    //添加会话
    public void remove(Session session) {
        sessions.remove(session.getId());
    }

    public Session getSession(String id) {
        return sessions.get(id);
    }

    public Map<String, Session> getSessions() {
        return sessions;
    }
}
