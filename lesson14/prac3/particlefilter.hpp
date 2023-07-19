#pragma once

#include <iostream>
#include <vector>
#include <random>

#include <opencv2/opencv.hpp>

// Particle class ==================================================================
class Particle {
public:
	double weight;	// 重み

	int height;
	int width;
	int height_speed;
	int width_speed;

	Particle();
	Particle(
		int _height,
		int _width,
		int _height_speed,
		int _width_speed
	);
};

// ParticleFilter class ==================================================================
class ParticleFilter{
private:
	int num;				// particleの個数
	int width;				// 横
	int height;				// 縦
	std::vector<int> upper;	// 最大値
	std::vector<int> lower;	// 最小値
	std::vector<int> noise;	// ノイズ
	std::vector<Particle> particles;

public:
	ParticleFilter();
	ParticleFilter(
		int _num, 
		int _height,
		int _width,
		std::vector<int> &_upper,
		std::vector<int> &_lower,
		std::vector<int> &_noise
	);

	void Predict();
	void CalcWeight(cv::Mat &src);
	void Resampling();
	double Likelifood(int x, int y, cv::Mat &src);

	std::vector<Particle> GetParticles();
};