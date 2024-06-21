package com.example.demo.service.Impl;



import com.example.demo.dao.PatientDao;
import com.example.demo.entity.Patient;
import com.example.demo.service.PatientService;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

@Service("patientService")
public class PatientServiceImpl implements PatientService {

    @Resource
    private PatientDao patientDao;

    @Override
    public Patient save(Patient patient) {
        return patientDao.save(patient);
    }

    @Override
    public Patient get(Long id) {
        return patientDao.findById(id).orElse(null);
    }

    @Override
    public List<Patient> getAll() {
        return patientDao.findAll();
    }

    @Override
    public void delete(Long id) {
        patientDao.deleteById(id);
    }

    @Override
    public Patient update(Long id, Patient patient) {
        Patient existingPatient = get(id);
        if (existingPatient != null) {
            existingPatient.setPatientname(patient.getPatientname());
            existingPatient.setPatientsex(patient.getPatientsex());
            existingPatient.setCaregiversid(patient.getCaregiversid());
            existingPatient.setDependentsid(patient.getDependentsid());
            return patientDao.save(existingPatient);
        }
        return null;
    }
}

