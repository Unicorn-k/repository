package com.example.demo.entity;

import javax.persistence.*;

@Entity
@Table(name = "dependents")
public class Dependents {
    @Id
    @GeneratedValue
    private Long dependentsid;
    private String dependentspassword;
    @Column(length = 50)
    private String dependentsname;
    @Column(length = 5)
    private String dependentssex;
    private Long patientid;
    private String dependentsphonenumber;

    public Long getDependentsid() {
        return dependentsid;
    }

    public void setDependentsid(Long dependentsid) {
        this.dependentsid = dependentsid;
    }

    public String getDependentspassword() {
        return dependentspassword;
    }

    public void setDependentspassword(String dependentspassword) {
        this.dependentspassword = dependentspassword;
    }

    public String getDependentsname() {
        return dependentsname;
    }

    public void setDependentsname(String dependentsname) {
        this.dependentsname = dependentsname;
    }

    public String getDependentssex() {
        return dependentssex;
    }

    public void setDependentssex(String dependentssex) {
        this.dependentssex = dependentssex;
    }

    public Long getPatientid() {
        return patientid;
    }

    public void setPatientid(Long patientid) {
        this.patientid = patientid;
    }

    public String getDependentsphonenumber() {
        return dependentsphonenumber;
    }

    public void setDependentsphonenumber(String dependentsphonenumber) {
        this.dependentsphonenumber = dependentsphonenumber;
    }
}
