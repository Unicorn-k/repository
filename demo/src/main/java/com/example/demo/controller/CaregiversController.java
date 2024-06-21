package com.example.demo.controller;


import com.example.demo.entity.Caregivers;
import com.example.demo.service.CaregiversService;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/caregivers")
public class CaregiversController {

    @Resource
    private CaregiversService caregiversService;

    @PostMapping
    public Caregivers save(@RequestBody Caregivers caregivers) {
        return caregiversService.save(caregivers);
    }

    @GetMapping("/{id}")
    public Caregivers get(@PathVariable Long id) {
        return caregiversService.get(id);
    }

    @GetMapping
    public Map<String, List<Caregivers>> getAll() {
        Map<String, List<Caregivers>> response = new HashMap<>();
        response.put("caregivers", caregiversService.getAll());
        return response;
    }

    @DeleteMapping("/{id}")
    public Map<String, String> delete(@PathVariable Long id) {
        caregiversService.delete(id);
        Map<String, String> response = new HashMap<>();
        response.put("message", "Caregiver with id " + id + " deleted successfully");
        return response;
    }

    @PutMapping("/{id}")
    public Caregivers update(@PathVariable Long id, @RequestBody Caregivers caregivers) {
        return caregiversService.update(id, caregivers);
    }
}
