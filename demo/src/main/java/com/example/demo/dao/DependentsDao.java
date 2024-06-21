package com.example.demo.dao;

import com.example.demo.entity.Dependents;
import org.springframework.data.jpa.repository.JpaRepository;

public interface DependentsDao extends JpaRepository<Dependents, Long> {
}
