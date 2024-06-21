package com.example.demo.service.Impl;

import com.example.demo.dao.DataOfFeaturesDao;
import com.example.demo.entity.DataOfFeatures;
import com.example.demo.service.DataOfFeaturesService;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

@Service("dataOfFeaturesService")//标记这个类是一个服务类
public class DataOfFeaturesServiceImpl implements DataOfFeaturesService {

    /*用于指定依赖注入的注解。它可以在类中标记一个字段或setter方法，使Spring容器自动装配相应的依赖项
    @Resource注解的作用是自动将DataOfFeaturesDao的实例注入到dataOfFeaturesDao这个私有变量中。
    这样就可以在这个类的其他方法中直接使用dataOfFeaturesDao来调用DataOfFeaturesDao接口的方法，
    而不需要自己去创建DataOfFeaturesDao的实例。
    */
    @Resource
    private DataOfFeaturesDao dataOfFeaturesDao;

    @Override
    public DataOfFeatures save(DataOfFeatures features) {
        return dataOfFeaturesDao.save(features);
    }

    @Override
    public DataOfFeatures get(Long id) {
        return dataOfFeaturesDao.findById(id).orElse(null);
    }

    @Override
    public List<DataOfFeatures> getAll() {
        return dataOfFeaturesDao.findAll();
    }

    @Override
    public void delete(Long id) {
        dataOfFeaturesDao.deleteById(id);
    }

    @Override
    public DataOfFeatures update(Long id, DataOfFeatures features) {
        DataOfFeatures existingStudent = get(id);
        if (existingStudent != null) {
            existingStudent.setName(features.getName());
            existingStudent.setSex(features.getSex());
            existingStudent.setTemp(features.getTemp());
            existingStudent.setHum(features.getHum());
            existingStudent.setHeartrate(features.getHeartrate());
            existingStudent.setSpo2(features.getSpo2());
            existingStudent.setMyDateTime(features.getMyDateTime());
            return dataOfFeaturesDao.save(existingStudent);
        }
        return null;
    }

}
