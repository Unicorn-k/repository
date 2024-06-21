package com.example.demo.entity;

import javax.persistence.*;

@Entity
@Table(name = "caregivers")
public class Caregivers {
    @Id
    //@GeneratedValue
    private Long caregiversid;
    private String caregiverspassword;
    @Column(length = 50)
    private String caregiversname;
    @Column(length = 5)
    private String caregiverssex;
    private Long patientid;
    private String caregiversphonenumber;

    public Long getCaregiversid() {
        return caregiversid;
    }

    public void setCaregiversid(Long caregiversid) {
        this.caregiversid = caregiversid;
    }

    public String getCaregiverspassword() {
        return caregiverspassword;
    }

    public void setCaregiverspassword(String caregiverspassword) {
        this.caregiverspassword = caregiverspassword;
    }

    public String getCaregiversname() {
        return caregiversname;
    }

    public void setCaregiversname(String caregiversname) {
        this.caregiversname = caregiversname;
    }

    public String getCaregiverssex() {
        return caregiverssex;
    }

    public void setCaregiverssex(String caregiverssex) {
        this.caregiverssex = caregiverssex;
    }

    public Long getPatientid() {
        return patientid;
    }

    public void setPatientid(Long patientid) {
        this.patientid = patientid;
    }

    public String getCaregiversphonenumber() {
        return caregiversphonenumber;
    }

    public void setCaregiversphonenumber(String caregiversphonenumber) {
        this.caregiversphonenumber = caregiversphonenumber;
    }
}
