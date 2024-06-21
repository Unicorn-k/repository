package com.example.demo.entity;

import javax.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "bodyfeatures")
public class DataOfFeatures {
    @Id
    @GeneratedValue
    private Long id;
    private long patientid;//Long
    private long deviceid;//Long
    @Column(length = 50)
    private String name;
    @Column(length = 5)
    private String sex;
    private Double temp;
    private Double hum;
    private int heartrate;
    private int spo2;
    private LocalDateTime myDateTime;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public long getPatientid() {
        return patientid;
    }

    public void setPatientid(long patientid) {
        this.patientid = patientid;
    }

    public long getDeviceid() {
        return deviceid;
    }

    public void setDeviceid(long deviceid) {
        this.deviceid = deviceid;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

    public Double getTemp() {
        return temp;
    }

    public void setTemp(Double temp) {
        this.temp = temp;
    }

    public Double getHum() {
        return hum;
    }

    public void setHum(Double hum) {
        this.hum = hum;
    }

    public int getHeartrate() {
        return heartrate;
    }

    public void setHeartrate(int heartrate) {
        this.heartrate = heartrate;
    }

    public int getSpo2() {
        return spo2;
    }

    public void setSpo2(int spo2) {
        this.spo2 = spo2;
    }

    public LocalDateTime getMyDateTime() {
        return myDateTime;
    }

    public void setMyDateTime(LocalDateTime myDateTime) {
        this.myDateTime = myDateTime;
    }
}
