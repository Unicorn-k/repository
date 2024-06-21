package com.example.demo.service;

import com.example.demo.entity.Dependents;

import java.util.List;

public interface DependentsService {
    public Dependents save(Dependents dependents);
    public Dependents get(Long id);
    public List<Dependents> getAll();
    public void delete(Long id);
    public Dependents update(Long id, Dependents dependents);
}
