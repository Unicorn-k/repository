package com.example.demo.mqtt;

import com.example.demo.entity.DataOfFeatures;
import com.example.demo.service.DataOfFeaturesService;
import com.example.demo.util.JsonDisipose;
import com.example.demo.util.SpringApplicationContext;
import com.example.demo.websocket.WsServerEndpoint;
import org.eclipse.paho.client.mqttv3.IMqttMessageListener;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.stereotype.Component;

import javax.annotation.Resource;
import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;
import java.util.Map;

//标记MessageCallbackListener类为一个Spring管理的组件。当Spring应用启动时，
//它会扫描项目中所有的类，找到标记为@Component的类，并为这些类创建对象实例，这些实例被称为beans。
@Component//创建一个实例--bean
public class MessageCallbackListener implements IMqttMessageListener {//接收mqtt的message

    @Resource
    private DataOfFeaturesService dataOfFeaturesService;
    private WsServerEndpoint wsServerEndpoint;//WebSocket服务器端点的实例


    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {
        String messageBody = new String(message.getPayload(), StandardCharsets.UTF_8);
        System.out.println("收到消息："+topic+", 消息内容是："+ messageBody);

        //esp———to——app------------------
        if (wsServerEndpoint == null) {
            wsServerEndpoint = SpringApplicationContext.getBean(WsServerEndpoint.class);
        }

        //-----解析json
        Map<String, Double> values = JsonDisipose.extractValues(messageBody);
        Double temp=values.get("temp");
        Double hum=values.get("hum");
        int heartrate=(int) values.get("heartrate").doubleValue();
        int spo2=(int) values.get("spo2").doubleValue();

        System.out.println("temp is:"+temp);
        System.out.println("hum is:"+hum);
        System.out.println("heartrate is:"+heartrate);
        System.out.println("spo2 is:"+spo2);
        //---------存储至数据库???
        // 创建一个 DataOfFeatures 对象并设置其属性
        // 获取 DataOfFeaturesService 的实例
        if (dataOfFeaturesService == null) {
            dataOfFeaturesService = SpringApplicationContext.getBean(DataOfFeaturesService.class);
        }
        DataOfFeatures features = new DataOfFeatures();
        //features.setId(1L);
        features.setPatientid(1L);
        features.setDeviceid(1L);
        features.setName("张三");
        features.setSex("男");
        features.setTemp(temp);
        features.setHum(hum);
        features.setHeartrate(heartrate);
        features.setSpo2(spo2);
        features.setMyDateTime(LocalDateTime.now());
        // 调用 DataOfFeaturesService 的 save 方法来保存数据
        try {
            System.out.println("Saving features to database...");
            dataOfFeaturesService.save(features);
            System.out.println("Features saved successfully.");
        } catch (Exception e) {
            System.out.println("Failed to save features to database.");
            e.printStackTrace();
        }
        //dataOfFeaturesService.save(features);


        //----重新封装json
        String jsonresult=JsonDisipose.createJson("iot-1",temp,hum,heartrate,spo2);
        System.out.println("result is"+jsonresult);

        //wsServerEndpoint.broadcast(messageBody);
        wsServerEndpoint.broadcast(jsonresult);
        //--------------------------------
    }
}


