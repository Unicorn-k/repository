package com.example.demo.service.Impl;


import com.example.demo.dao.MattersDao;
import com.example.demo.entity.Matters;
import com.example.demo.service.MattersService;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

@Service("mattersService")
public class MattersServiceImpl implements MattersService {

    @Resource
    private MattersDao mattersDao;

    @Override
    public Matters save(Matters matters) {
        return mattersDao.save(matters);
    }

    @Override
    public Matters get(Long id) {
        return mattersDao.findById(id).orElse(null);
    }

    @Override
    public List<Matters> getAll() {
        return mattersDao.findAll();
    }

    @Override
    public void delete(Long id) {
        mattersDao.deleteById(id);
    }

    @Override
    public Matters update(Long id, Matters matters) {
        Matters existingMatters = get(id);
        if (existingMatters != null) {
            existingMatters.setPatientid(matters.getPatientid());
            existingMatters.setPatientname(matters.getPatientname());
            existingMatters.setPatientsex(matters.getPatientsex());
            existingMatters.setMattersname(matters.getMattersname());
            existingMatters.setMattersontime(matters.getMattersontime());
            existingMatters.setMatterscontext(matters.getMatterscontext());
            existingMatters.setMattersstate(matters.getMattersstate());
            existingMatters.setMattersfinishtime(matters.getMattersfinishtime());
            return mattersDao.save(existingMatters);
        }
        return null;
    }
}

