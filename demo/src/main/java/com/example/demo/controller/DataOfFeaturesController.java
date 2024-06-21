package com.example.demo.controller;

import com.example.demo.entity.DataOfFeatures;
import com.example.demo.service.DataOfFeaturesService;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/features")
public class DataOfFeaturesController {

    @Resource
    private DataOfFeaturesService dataOfFeaturesService;

    @PostMapping
    public DataOfFeatures save(@RequestBody DataOfFeatures features) {
        return dataOfFeaturesService.save(features);
    }

    @GetMapping("/{id}")
    public DataOfFeatures get(@PathVariable Long id) {
        return dataOfFeaturesService.get(id);
    }

    //json数组
//    @GetMapping
//    public List<DataOfFeatures> getAll() {
//        return dataOfFeaturesService.getAll();
//    }
    //json对象
    @GetMapping
    public Map<String, List<DataOfFeatures>> getAll() {
        Map<String, List<DataOfFeatures>> response = new HashMap<>();
        response.put("features", dataOfFeaturesService.getAll());
        return response;
    }


//    @DeleteMapping("/{id}")
//    public void delete(@PathVariable Long id) {
//        dataOfFeaturesService.delete(id);
//    }
    @DeleteMapping("/{id}")
    public Map<String, String> delete(@PathVariable Long id) {
        dataOfFeaturesService.delete(id);
        Map<String, String> response = new HashMap<>();
        response.put("message", "DataOfFeatures with id " + id + " deleted successfully");
        return response;
    }

    @PutMapping("/{id}")
    public DataOfFeatures update(@PathVariable Long id, @RequestBody DataOfFeatures features) {
        return dataOfFeaturesService.update(id, features);
    }

}
