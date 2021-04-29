#include <iostream>
#include <vector>


typedef size_t profile;
typedef std::vector<size_t> vector_st;


// Определяет есть ли домино на bit_number месте в профиле
bool IsCovered(const profile& profile_, size_t bit_number) {
    return profile_ & (1 << bit_number);
}


// Находит профили, в которые можно попасть из профиля prof
void WhereToGo(const profile& profile_, profile next_profile, size_t bit_number,
               std::vector<vector_st>& profile_relations, const size_t& N) {
    if (bit_number == N) {
        profile_relations[profile_][next_profile] = 1;
        return;
    }

    if (!IsCovered(profile_, bit_number)) {
        WhereToGo(profile_, next_profile | (1 << bit_number), bit_number + 1, profile_relations, N);
        if (bit_number < N - 1 && !IsCovered(profile_, bit_number + 1)) {
            WhereToGo(profile_, next_profile, bit_number + 2, profile_relations, N);
        }
    } else {
        WhereToGo(profile_, next_profile, bit_number + 1, profile_relations, N);
    }
}


// Заполняет таблицу отношений профилей
void BuildProfileRelation(std::vector<vector_st>& profile_relations, const size_t& N) {
    for (profile profile_ = 0; profile_ < 1 << N; ++profile_) {
        WhereToGo(profile_, 0, 0, profile_relations, N);
    }
}


// Считает количество вариантов заполнить поле
size_t HowManyVariants(std::vector<vector_st>& profile_relation, const size_t& M, const size_t& profile_count) {
    // Количество вариантов замостить k колонку так, чтобы в ней был определенный профиль
    // Храним только количество вариантов для последних двух колонок
    std::vector<vector_st> counters(2, vector_st(profile_count, 0));
    counters[0][0] = 1;

    for (size_t k = 1; k <= M; ++k) {
        for (profile i = 0; i < profile_count; ++i) {
            for (profile j = 0; j < profile_count; ++j) {
                counters[k % 2][i] += counters[1 - k % 2][j] * profile_relation[j][i];
            }
        }
        for (auto &count : counters[1 - k % 2]) {
            count = 0;
        }
    }

    return counters[M % 2][0];
}


int main() {
    size_t N, M; // размеры поля
    std::cin >> N >> M;

    size_t profile_count = 1 << N; // всего есть 2^N профилей

    // Таблица отношений между профилями
    std::vector<vector_st> profile_relation(profile_count, vector_st(profile_count, 0));

    BuildProfileRelation(profile_relation, N);

    std::cout << HowManyVariants(profile_relation, M, profile_count) << '\n';

    return 0;
}
