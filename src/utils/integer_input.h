#include <climits>
#include <stdexcept>
#include <string>

#include "ftxui/component/component.hpp"

using namespace ftxui;

inline Component IntegerInput(int* n, const int def = 0, const int min = INT_MIN, const int max = INT_MAX) {
    class Impl : public ComponentBase {
    public:
        Impl(int* n, const int def, const int min, const int max) : prev_(""), str_(""), n_(n) {
            InputOption option;
            option.cursor_position = &cur_;
            option.on_change = [this, def, min, max] {
                if (str_ == "--") {
                    str_ = "";
                }
                if (str_.empty() || (min < 0 && str_ == "-")) {
                    *n_ = def;
                    prev_ = "";
                    return;
                }
                int i = 0;
                if (min < 0 && str_[0] == '-' && str_.size() > 1) {
                    i = 1;
                }
                for (; i < str_.size(); i++) {
                    if (!isdigit(str_[i])) {
                        if (min < 0 && str_[i] == '-' && i == str_.size() - 1) {
                            str_ = str_[0] == '-' ? str_.substr(1, str_.size() - 2) : "-" + str_.substr(0, str_.size() - 1);
                            break;
                        }
                        str_ = prev_;
                        cur_--;
                        return;
                    }
                }
                try {
                    *n_ = std::stoi(str_);
                } catch (std::out_of_range) {
                    *n_ = str_[0] == '-' ? min : max;
                }
                if (*n_ < min) {
                    *n_ = min;
                } else if (*n_ > max) {
                    *n_ = max;
                }
                str_ = std::to_string(*n_);
                prev_ = str_;
            };
            input_ = Input(&str_, std::to_string(def), option);
            Add(input_);
        }

    private:
        std::string prev_;
        std::string str_;
        int* n_;
        int cur_;
        Component input_;
    };
    return Make<Impl>(n, def, min, max);
}