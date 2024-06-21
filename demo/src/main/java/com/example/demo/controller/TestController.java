package com.example.demo.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {
    @GetMapping("/test")
    public String test() {
        return "OKay";
    }
    @PostMapping("/post")
    public void post(@RequestBody String data) {
        System.out.println("Received data from ESP8266: " + data);
    }
}


//http----------可以删除这个类