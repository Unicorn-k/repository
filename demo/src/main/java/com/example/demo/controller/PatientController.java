package com.example.demo.controller;


import com.example.demo.entity.Patient;
import com.example.demo.service.PatientService;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/patient")
public class PatientController {

    @Resource
    private PatientService patientService;

    @PostMapping
    public Patient save(@RequestBody Patient patient) {
        return patientService.save(patient);
    }

    @GetMapping("/{id}")
    public Patient get(@PathVariable Long id) {
        return patientService.get(id);
    }

    @GetMapping
    public Map<String, List<Patient>> getAll() {
        Map<String, List<Patient>> response = new HashMap<>();
        response.put("patients", patientService.getAll());
        return response;
    }

    @DeleteMapping("/{id}")
    public Map<String, String> delete(@PathVariable Long id) {
        patientService.delete(id);
        Map<String, String> response = new HashMap<>();
        response.put("message", "Patient with id " + id + " deleted successfully");
        return response;
    }

    @PutMapping("/{id}")
    public Patient update(@PathVariable Long id, @RequestBody Patient patient) {
        return patientService.update(id, patient);
    }
}

