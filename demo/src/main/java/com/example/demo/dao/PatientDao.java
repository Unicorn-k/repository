package com.example.demo.dao;

import com.example.demo.entity.Patient;
import org.springframework.data.jpa.repository.JpaRepository;

public interface PatientDao extends JpaRepository<Patient, Long> {
}
