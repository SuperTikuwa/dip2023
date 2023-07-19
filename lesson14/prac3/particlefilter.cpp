#include "particlefilter.hpp"

#include <iostream>

// Particle class ==================================================================
/**
 * Particleクラスの引数なしコンストラクタ
*/
Particle::Particle()
{
	Particle(0, 0, 0, 0);
}

/**
 * Particleクラスの引数ありコンストラクタ
*/
Particle::Particle(int _height, int _width, int _height_speed, int _width_speed)
{
	height = _height;
	width = _width;
	height_speed = _height_speed;
	width_speed = _width_speed;
}

// ParticleFilter class ==================================================================
/**
 * ParticleFilterクラスの引数なしコンストラクタ
*/
ParticleFilter::ParticleFilter()
{
    int _rand = rand() * 100 + 1;
    std::vector<int> upper = { _rand, _rand, 10, 10 };
    std::vector<int> lower = { 0, 0, -10, -10 };
    std::vector<int> noise = { 30, 30, 10, 10 };
    ParticleFilter(100, _rand, _rand, upper, lower, noise);
}

/**
 * ParticleFilterクラスの引数ありコンストラクタ
*/
ParticleFilter::ParticleFilter(
	int _num, 
	int _height,
	int _width,
	std::vector<int> &_upper,
	std::vector<int> &_lower,
	std::vector<int> &_noise)
{
	num = _num;
	height = _height;
	width = _width;
	upper = _upper;
	lower = _lower;
	noise = _noise;

	for (int i = 0;i < num;i++) {
		int n[4];
		for (int j = 0;j < 4;j++) {
			int x = (int)(((double)rand() / RAND_MAX) * INT_MAX);
			n[j] = x % (upper[j] - lower[j]) + lower[j];
		}
		Particle particle(n[0], n[1], n[2], n[3]);

		particles.push_back(particle);
		particles[i].weight = 1.0 / num;
	}
}

/**
 * 物体の位置を予測する
 * Arg: なし
 * Ret: なし
*/
void ParticleFilter::Predict() {
	for (int i = 0;i < num;i++) {
		int n[4];
		for (int j = 0;j < 4;j++) n[j] = (int)(((double)rand() / RAND_MAX) * INT_MAX) % (noise[j] * 2) - noise[j];

		particles[i].width += particles[i].width_speed + n[0];
        particles[i].height += particles[i].height_speed + n[1];
        particles[i].width_speed += n[2];
        particles[i].height_speed += n[3];

		if (particles[i].width < lower[0]) particles[i].width = lower[0];
        if (particles[i].height < lower[1]) particles[i].height = lower[1];
        if (particles[i].width_speed < lower[2]) particles[i].width_speed = lower[2];
        if (particles[i].height_speed < lower[3]) particles[i].height_speed = lower[3];

        if (particles[i].width >= upper[0]) particles[i].width = upper[0];
        if (particles[i].height >= upper[1]) particles[i].height = upper[1];
        if (particles[i].width_speed >= upper[2]) particles[i].width_speed = upper[2];
        if (particles[i].height_speed >= upper[3]) particles[i].height_speed = upper[3];
	}
}

/**
 * 重みを計算する
 * Arg: 対象画像
 * Ret: なし
*/
void ParticleFilter::CalcWeight(cv::Mat &src) {
    double sum_weight = 0.0;

    for (int i = 0; i < particles.size();i++) {
        int x = particles[i].width;
        int y = particles[i].height;

        //尤度の計算
        if (x < 0 || x >= src.size().width || y < 0 || y >= src.size().height)
            particles[i].weight = 0.001;
        else
            particles[i].weight = Likelifood(x, y, src);

        sum_weight += particles[i].weight;
    }

    //正規化
    for (int i = 0; i < particles.size(); ++i) particles[i].weight /= sum_weight;
}

/**
 * リサンプリングする
 * Arg: なし
 * Ret: なし
*/
void ParticleFilter::Resampling() {
    std::vector<double> sum_weight(num);
    sum_weight[0] = particles[0].weight;
    for (int i = 1; i < num;i++) {
        sum_weight[i] = sum_weight[i - 1] + particles[i].weight;
    }

    std::vector<Particle> pre_particle(particles);
    for (int i = 0; i < num;i++) {
        double weight_threshold = (double)(rand() % 10000) / 10000.0;
        for (int j = 0; j < num;j++) {
            if (weight_threshold > sum_weight[j]) {
                continue;
            } else {
                particles[i] = pre_particle[j];
                particles[i].weight = 0.0;
                break;
            }
        }
    }
}

/**
 * 尤度を計算する
 * Arg: x: ---+
 *      y: ---+-- 計算対象画素の座標
 *      src: 計算対象の画像
 * Ret: 尤度
*/
double ParticleFilter::Likelifood(int x, int y, cv::Mat &src) {
    float b, g, r;
    float dist = 0.0, sigma = 50.0;

    b = src.data[y * src.step + x * src.elemSize() + 0];    //B
    g = src.data[y * src.step + x * src.elemSize() + 1];    //G
    r = src.data[y * src.step + x * src.elemSize() + 2];    //R

    dist = sqrt(b * b + (255.0 - g) * (255.0 - g) + r * r);

    return 1.0 / (sqrt(2.0 * CV_PI) * sigma) * expf(-dist * dist / (2.0 * sigma * sigma));
}

/**
 * パーティクルを取得する
 * Arg: なし
 * Ret: パーティクルが格納された可変長配列
*/
std::vector<Particle> ParticleFilter::GetParticles() {
    return particles;
}
