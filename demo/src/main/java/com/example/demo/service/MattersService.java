package com.example.demo.service;

import com.example.demo.entity.Matters;
import java.util.List;

public interface MattersService {
    public Matters save(Matters matters);
    public Matters get(Long id);
    public List<Matters> getAll();
    public void delete(Long id);
    public Matters update(Long id, Matters matters);
}
