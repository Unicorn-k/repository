package com.example.demo.service.Impl;

import com.example.demo.dao.CaregiversDao;
import com.example.demo.entity.Caregivers;
import com.example.demo.service.CaregiversService;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

@Service("caregiversService")
public class CaregiversServiceImpl implements CaregiversService {

    @Resource
    private CaregiversDao caregiversDao;

    @Override
    public Caregivers save(Caregivers caregivers) {
        return caregiversDao.save(caregivers);
    }

    @Override
    public Caregivers get(Long id) {
        return caregiversDao.findById(id).orElse(null);
    }

    @Override
    public List<Caregivers> getAll() {
        return caregiversDao.findAll();
    }

    @Override
    public void delete(Long id) {
        caregiversDao.deleteById(id);
    }

    @Override
    public Caregivers update(Long id, Caregivers caregivers) {
        Caregivers existingCaregiver = get(id);
        if (existingCaregiver != null) {
            existingCaregiver.setCaregiverspassword(caregivers.getCaregiverspassword());
            existingCaregiver.setCaregiversname(caregivers.getCaregiversname());
            existingCaregiver.setCaregiverssex(caregivers.getCaregiverssex());
            existingCaregiver.setPatientid(caregivers.getPatientid());
            existingCaregiver.setCaregiversphonenumber(caregivers.getCaregiversphonenumber());
            return caregiversDao.save(existingCaregiver);
        }
        return null;
    }
}
