package com.example.demo.controller;


import com.example.demo.entity.Matters;
import com.example.demo.service.MattersService;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/matters")
public class MattersController {

    @Resource
    private MattersService mattersService;

    @PostMapping
    public Matters save(@RequestBody Matters matters) {
        return mattersService.save(matters);
    }

    @GetMapping("/{id}")
    public Matters get(@PathVariable Long id) {
        return mattersService.get(id);
    }

    @GetMapping
    public Map<String, List<Matters>> getAll() {
        Map<String, List<Matters>> response = new HashMap<>();
        response.put("matters", mattersService.getAll());
        return response;
    }

    @DeleteMapping("/{id}")
    public Map<String, String> delete(@PathVariable Long id) {
        mattersService.delete(id);
        Map<String, String> response = new HashMap<>();
        response.put("message", "Matters with id " + id + " deleted successfully");
        return response;
    }

    @PutMapping("/{id}")
    public Matters update(@PathVariable Long id, @RequestBody Matters matters) {
        return mattersService.update(id, matters);
    }
}

