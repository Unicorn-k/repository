package com.example.demo.entity;

import javax.persistence.*;

@Entity
@Table(name = "patient")
public class Patient {
    @Id
    @GeneratedValue
    private Long patientid;
    @Column(length = 50)
    private String patientname;
    @Column(length = 5)
    private String patientsex;
    private Long caregiversid;
    private Long dependentsid;

    public Long getPatientid() {
        return patientid;
    }

    public void setPatientid(Long patientid) {
        this.patientid = patientid;
    }

    public String getPatientname() {
        return patientname;
    }

    public void setPatientname(String patientname) {
        this.patientname = patientname;
    }

    public String getPatientsex() {
        return patientsex;
    }

    public void setPatientsex(String patientsex) {
        this.patientsex = patientsex;
    }

    public Long getCaregiversid() {
        return caregiversid;
    }

    public void setCaregiversid(Long caregiversid) {
        this.caregiversid = caregiversid;
    }

    public Long getDependentsid() {
        return dependentsid;
    }

    public void setDependentsid(Long dependentsid) {
        this.dependentsid = dependentsid;
    }
}
