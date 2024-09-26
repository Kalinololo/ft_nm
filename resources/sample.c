int global_var = 42;
static int static_var = 7;

void func1() {
    return ;
}

static void __func2() {
    return ;
}

int main() {
    func1();
    __func2();
    return 0;
}