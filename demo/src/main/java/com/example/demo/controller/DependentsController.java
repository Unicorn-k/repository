package com.example.demo.controller;


import com.example.demo.entity.Dependents;
import com.example.demo.service.DependentsService;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/dependents")
public class DependentsController {

    @Resource
    private DependentsService dependentsService;

    @PostMapping
    public Dependents save(@RequestBody Dependents dependents) {
        return dependentsService.save(dependents);
    }

    @GetMapping("/{id}")
    public Dependents get(@PathVariable Long id) {
        return dependentsService.get(id);
    }

    @GetMapping
    public Map<String, List<Dependents>> getAll() {
        Map<String, List<Dependents>> response = new HashMap<>();
        response.put("dependents", dependentsService.getAll());
        return response;
    }

    @DeleteMapping("/{id}")
    public Map<String, String> delete(@PathVariable Long id) {
        dependentsService.delete(id);
        Map<String, String> response = new HashMap<>();
        response.put("message", "Dependent with id " + id + " deleted successfully");
        return response;
    }

    @PutMapping("/{id}")
    public Dependents update(@PathVariable Long id, @RequestBody Dependents dependents) {
        return dependentsService.update(id, dependents);
    }
}

