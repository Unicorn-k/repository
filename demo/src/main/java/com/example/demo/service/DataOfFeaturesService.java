package com.example.demo.service;

import com.example.demo.entity.DataOfFeatures;

import java.util.List;

public interface DataOfFeaturesService {
    public DataOfFeatures save(DataOfFeatures features);
    public DataOfFeatures get(Long id);
    public List<DataOfFeatures> getAll();
    public void delete(Long id);
    public DataOfFeatures update(Long id, DataOfFeatures features);

}
