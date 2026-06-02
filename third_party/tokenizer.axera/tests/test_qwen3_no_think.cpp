#include "../include/BaseTokenizer.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

bool check_suffix(const std::string &text, const std::string &suffix)
{
    return text.size() >= suffix.size() &&
           text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int run_case(const std::string &tokenizer_name,
             bool think_in_prompt,
             const std::string &expected_suffix)
{
    auto tokenizer = create_tokenizer(tokenizer_name);
    if (!tokenizer)
    {
        std::cerr << "failed to create tokenizer: " << tokenizer_name << '\n';
        return 1;
    }

    tokenizer->set_think_in_prompt(think_in_prompt);
    std::vector<Content> contents = {
        {SYSTEM, TEXT, "system"},
        {USER, TEXT, "hello"},
    };

    const std::string templ = tokenizer->apply_chat_template(contents, true);
    if (!check_suffix(templ, expected_suffix))
    {
        std::cerr << "unexpected suffix for " << tokenizer_name
                  << " think_in_prompt=" << think_in_prompt << "\nexpected: "
                  << expected_suffix << "\nactual: " << templ << '\n';
        return 1;
    }

    return 0;
}

} // namespace

int main()
{
    int failed = 0;
    failed += run_case("Qwen3", false, "<|im_start|>assistant\n<think>\n\n</think>\n\n");
    failed += run_case("Qwen3", true, "<|im_start|>assistant\n");
    failed += run_case("Qwen3VL", false, "<|im_start|>assistant\n<think>\n\n</think>\n\n");
    failed += run_case("Qwen3_5", false, "<|im_start|>assistant\n<think>\n\n</think>\n\n");
    failed += run_case("Qwen2_5", false, "<|im_start|>assistant\n");

    if (failed != 0)
    {
        std::cerr << "test_qwen3_no_think failed: " << failed << " case(s)\n";
        return 1;
    }

    std::cout << "test_qwen3_no_think passed\n";
    return 0;
}
