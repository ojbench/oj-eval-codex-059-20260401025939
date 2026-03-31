// Implementation for Problem 059 - OJ 的继承与派生
#include <cstring>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>

class BaseJudger {
public:
    BaseJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : answer_(nullptr), time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        if (answer) {
            size_t n = std::strlen(answer);
            answer_ = new char[n + 1];
            std::memcpy(answer_, answer, n + 1);
        } else {
            answer_ = new char[1];
            answer_[0] = '\0';
        }
    }

    virtual void Submit(size_t time, size_t memory, const char *output) = 0;

    size_t GetScore() const { return score_; }

    virtual ~BaseJudger() {
        delete[] answer_;
        answer_ = nullptr;
    }

protected:
    char *answer_;
    const size_t time_limit_;
    const size_t memory_limit_;
    size_t score_;

    virtual bool CheckAnswer(const char *output) const {
        return std::strcmp(answer_, output ? output : "") == 0;
    }
};

class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        size_t cur = 0;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            cur = 100;
        } else {
            cur = 0;
        }
        if (cur > score_) score_ = cur;
    }
};

class OIJudger : public BaseJudger {
public:
    OIJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), submitted_(false) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (submitted_) {
            // Multiple submissions lead to zero score
            score_ = 0;
            return;
        }
        submitted_ = true;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        } else {
            score_ = 0;
        }
    }

protected:
    bool CheckAnswer(const char *output) const override {
        const char *out = output ? output : "";
        // Compare by lines, ignoring trailing spaces of each line
        return Normalize(answer_) == Normalize(out);
    }

private:
    bool submitted_;

    static std::vector<std::string> SplitLines(const char *s) {
        std::vector<std::string> lines;
        if (!s) return lines;
        const char *p = s;
        const char *start = p;
        while (*p) {
            if (*p == '\n') {
                lines.emplace_back(start, p - start);
                ++p;
                start = p;
            } else {
                ++p;
            }
        }
        // Capture last line if no trailing newline
        if (p != start) {
            lines.emplace_back(start, p - start);
        }
        return lines;
    }

    static void RTrimSpaces(std::string &s) {
        while (!s.empty() && (s.back() == ' ' || s.back() == '\r')) {
            s.pop_back();
        }
    }

    static std::vector<std::string> Normalize(const char *s) {
        std::vector<std::string> lines = SplitLines(s);
        for (auto &ln : lines) {
            RTrimSpaces(ln);
        }
        return lines;
    }
};

class SpacialJudger : public BaseJudger {
public:
    SpacialJudger(size_t time_limit, size_t memory_limit,
                  size_t full_score_time, size_t full_score_memory,
                  const char *answer)
        : BaseJudger(time_limit, memory_limit, answer),
          full_score_time_(full_score_time), full_score_memory_(full_score_memory) {}

    void Submit(size_t time, size_t memory, const char *output) override {
        if (!CheckAnswer(output)) {
            if (score_ < 0) score_ = 0; // no-op; maintain non-negative
            // wrong answer yields 0 for this submission; keep best
            return; // score_ remains previous best (0 by default)
        }
        int ts = ComputeScore(time, full_score_time_, time_limit_);
        int ms = ComputeScore(memory, full_score_memory_, memory_limit_);
        int combined = (ts * ms) / 100;
        if ((size_t)combined > score_) score_ = (size_t)combined;
    }

private:
    size_t full_score_time_;
    size_t full_score_memory_;

    static int ComputeScore(size_t val, size_t full_score_val, size_t limit_val) {
        if (val <= full_score_val) return 100;
        if (val >= limit_val) return 0;
        // Linear interpolation: floor
        // score = floor((limit - val) * 100 / (limit - full))
        long long num = static_cast<long long>(limit_val) - static_cast<long long>(val);
        long long den = static_cast<long long>(limit_val) - static_cast<long long>(full_score_val);
        if (den <= 0) {
            // Avoid division by zero; if full_score_val >= limit_val, then only <= full gives 100, else 0
            return 0;
        }
        long long score = (num * 100) / den;
        if (score < 0) score = 0;
        if (score > 100) score = 100;
        return static_cast<int>(score);
    }
};

