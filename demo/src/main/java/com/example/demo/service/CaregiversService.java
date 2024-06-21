package com.example.demo.service;

import com.example.demo.entity.Caregivers;
import java.util.List;

public interface CaregiversService {
    public Caregivers save(Caregivers caregivers);
    public Caregivers get(Long id);
    public List<Caregivers> getAll();
    public void delete(Long id);
    public Caregivers update(Long id, Caregivers caregivers);
}
