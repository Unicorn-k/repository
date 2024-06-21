package com.example.demo.entity;

import javax.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "matters")
public class Matters {
    @Id
    @GeneratedValue
    private Long mattersid;
    private Long patientid;
    @Column(length = 50)
    private String patientname;
    @Column(length = 5)
    private String patientsex;
    @Column(length = 50)
    private String mattersname;
    private LocalDateTime mattersontime;
    @Column(length = 255)
    private String matterscontext;
    @Column(length = 50)
    private String mattersstate;
    private LocalDateTime mattersfinishtime;

    public Long getMattersid() {
        return mattersid;
    }

    public void setMattersid(Long mattersid) {
        this.mattersid = mattersid;
    }

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

    public String getMattersname() {
        return mattersname;
    }

    public void setMattersname(String mattersname) {
        this.mattersname = mattersname;
    }

    public LocalDateTime getMattersontime() {
        return mattersontime;
    }

    public void setMattersontime(LocalDateTime mattersontime) {
        this.mattersontime = mattersontime;
    }

    public String getMatterscontext() {
        return matterscontext;
    }

    public void setMatterscontext(String matterscontext) {
        this.matterscontext = matterscontext;
    }

    public String getMattersstate() {
        return mattersstate;
    }

    public void setMattersstate(String mattersstate) {
        this.mattersstate = mattersstate;
    }

    public LocalDateTime getMattersfinishtime() {
        return mattersfinishtime;
    }

    public void setMattersfinishtime(LocalDateTime mattersfinishtime) {
        this.mattersfinishtime = mattersfinishtime;
    }
}
