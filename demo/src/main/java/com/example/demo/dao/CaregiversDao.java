package com.example.demo.dao;

import com.example.demo.entity.Caregivers;
import org.springframework.data.jpa.repository.JpaRepository;

public interface CaregiversDao extends JpaRepository<Caregivers, Long> {
}
