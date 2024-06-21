package com.example.demo.service.Impl;


import com.example.demo.dao.DependentsDao;
import com.example.demo.entity.Dependents;
import com.example.demo.service.DependentsService;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

@Service("dependentsService")
public class DependentsServiceImpl implements DependentsService {

    @Resource
    private DependentsDao dependentsDao;

    @Override
    public Dependents save(Dependents dependents) {
        return dependentsDao.save(dependents);
    }

    @Override
    public Dependents get(Long id) {
        return dependentsDao.findById(id).orElse(null);
    }

    @Override
    public List<Dependents> getAll() {
        return dependentsDao.findAll();
    }

    @Override
    public void delete(Long id) {
        dependentsDao.deleteById(id);
    }

    @Override
    public Dependents update(Long id, Dependents dependents) {
        Dependents existingDependent = get(id);
        if (existingDependent != null) {
            existingDependent.setDependentspassword(dependents.getDependentspassword());
            existingDependent.setDependentsname(dependents.getDependentsname());
            existingDependent.setDependentssex(dependents.getDependentssex());
            existingDependent.setPatientid(dependents.getPatientid());
            existingDependent.setDependentsphonenumber(dependents.getDependentsphonenumber());
            return dependentsDao.save(existingDependent);
        }
        return null;
    }
}

