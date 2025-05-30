#ifndef SCOPEDFDREDIRECT_H
#define SCOPEDFDREDIRECT_H
#include <string>
#include <vector>


class ScopedFdRedirect {
public:
    explicit ScopedFdRedirect(std::vector<std::string>& tokens);
    ~ScopedFdRedirect();

private:
    int m_saved_in_fd, m_saved_out_fd, m_saved_err_fd;
    bool m_changed_in, m_changed_out, m_changed_err;
};



#endif //SCOPEDFDREDIRECT_H
