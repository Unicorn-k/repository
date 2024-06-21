package com.example.demo.util;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;

import java.time.Instant;
import java.util.HashMap;
import java.util.Map;

public class JsonDisipose {
    public static Map<String, Double> extractValues(String jsonString) {
        Map<String, Double> results = new HashMap<>();
        try {
            ObjectMapper objectMapper = new ObjectMapper();
            JsonNode rootNode = objectMapper.readTree(jsonString);

            JsonNode paramsNode = rootNode.path("params");

            double temp = paramsNode.path("temp").asDouble();
            double hum = paramsNode.path("hum").asDouble();
            int heartrate = paramsNode.path("heartrate").asInt();
            int spo2 = paramsNode.path("spo2").asInt();

            results.put("temp", temp);
            results.put("hum", hum);
            results.put("heartrate", (double) heartrate);
            results.put("spo2", (double) spo2);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return results;
    }

    public static String createJson(String id, double temp, double hum, int heartrate, int spo2) {
        try {
            ObjectMapper objectMapper = new ObjectMapper();
            ObjectNode rootNode = objectMapper.createObjectNode();

            rootNode.put("deviceType", "CustomCategory");
            rootNode.put("iotId", id);  // 使用变量 id 作为 iotId 的值
            rootNode.put("deviceName", "Mqtt_stm32");

            ObjectNode itemsNode = objectMapper.createObjectNode();
            long currentTime = Instant.now().toEpochMilli();

            ObjectNode tempNode = objectMapper.createObjectNode();
            tempNode.put("value", temp);
            tempNode.put("time", currentTime);
            itemsNode.set("temp", tempNode);

            ObjectNode humNode = objectMapper.createObjectNode();
            humNode.put("value", hum);
            humNode.put("time", currentTime);
            itemsNode.set("hum", humNode);

            ObjectNode heartrateNode = objectMapper.createObjectNode();
            heartrateNode.put("value", heartrate);
            heartrateNode.put("time", currentTime);
            itemsNode.set("heartrate", heartrateNode);

            ObjectNode spo2Node = objectMapper.createObjectNode();
            spo2Node.put("value", spo2);
            spo2Node.put("time", currentTime);
            itemsNode.set("spo2", spo2Node);

            rootNode.set("items", itemsNode);

            return rootNode.toString();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static Map<String, Integer> extractValuesofapp(String jsonString) {
        Map<String, Integer> results = new HashMap<>();
        try {
            ObjectMapper objectMapper = new ObjectMapper();
            JsonNode rootNode = objectMapper.readTree(jsonString);

            JsonNode paramsNode = rootNode.path("params");

            int workMode = paramsNode.path("WorkMode").asInt();
            int massageMode = paramsNode.path("MassageMode").asInt();
            int currentStrengthLeft = paramsNode.path("current_strength_left").asInt();
            int currentStrengthRight = paramsNode.path("current_strength_right").asInt();
            int channelLeft = paramsNode.path("channel_left").asInt();
            int channelRight = paramsNode.path("channel_right").asInt();

            results.put("WorkMode", workMode);
            results.put("MassageMode", massageMode);
            results.put("current_strength_left", currentStrengthLeft);
            results.put("current_strength_right", currentStrengthRight);
            results.put("channel_left", channelLeft);
            results.put("channel_right", channelRight);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return results;
    }

    public static String createJsonofapp(String iotId, int workMode, int massageMode, int currentStrengthLeft,
                                    int currentStrengthRight, int channelLeft, int channelRight) {
        try {
            ObjectMapper objectMapper = new ObjectMapper();
            ObjectNode rootNode = objectMapper.createObjectNode();

            rootNode.put("deviceType", "CustomCategory");
            rootNode.put("iotId", iotId);  // 使用变量 iotId 作为 iotId 的值
            rootNode.put("deviceName", "Mqtt_app");

            ObjectNode itemsNode = objectMapper.createObjectNode();
            long currentTime = Instant.now().toEpochMilli();

            ObjectNode workModeNode = objectMapper.createObjectNode();
            workModeNode.put("value", workMode);
            workModeNode.put("time", currentTime);
            itemsNode.set("WorkMode", workModeNode);

            ObjectNode massageModeNode = objectMapper.createObjectNode();
            massageModeNode.put("value", massageMode);
            massageModeNode.put("time", currentTime);
            itemsNode.set("MassageMode", massageModeNode);

            ObjectNode currentStrengthLeftNode = objectMapper.createObjectNode();
            currentStrengthLeftNode.put("value", currentStrengthLeft);
            currentStrengthLeftNode.put("time", currentTime);
            itemsNode.set("current_strength_left", currentStrengthLeftNode);

            ObjectNode currentStrengthRightNode = objectMapper.createObjectNode();
            currentStrengthRightNode.put("value", currentStrengthRight);
            currentStrengthRightNode.put("time", currentTime);
            itemsNode.set("current_strength_right", currentStrengthRightNode);

            ObjectNode channelLeftNode = objectMapper.createObjectNode();
            channelLeftNode.put("value", channelLeft);
            channelLeftNode.put("time", currentTime);
            itemsNode.set("channel_left", channelLeftNode);

            ObjectNode channelRightNode = objectMapper.createObjectNode();
            channelRightNode.put("value", channelRight);
            channelRightNode.put("time", currentTime);
            itemsNode.set("channel_right", channelRightNode);

            rootNode.set("items", itemsNode);

            return rootNode.toString();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
