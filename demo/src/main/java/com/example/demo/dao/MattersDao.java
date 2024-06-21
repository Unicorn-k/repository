package com.example.demo.dao;

import com.example.demo.entity.Matters;
import org.springframework.data.jpa.repository.JpaRepository;

public interface MattersDao extends JpaRepository<Matters, Long> {
}
