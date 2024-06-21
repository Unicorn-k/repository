package com.example.demo.service;

import com.example.demo.entity.Patient;

import java.util.List;

public interface PatientService {
    public Patient save(Patient patient);
    public Patient get(Long id);
    public List<Patient> getAll();
    public void delete(Long id);
    public Patient update(Long id, Patient patient);
}
