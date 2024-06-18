
# sudo apt install net-tools gcc make gdb plocate


# apt remove bash-completion
# sudo vi /etc/bash_completion
# sudo vi /etc/bash.bashrc


alias vienv='vi ~/.bash_aliases ; source ~/.bash_aliases'
alias ll='ls -la'
alias lr='ls -ltr'

export PS1='\u@SPR:\w\$ '
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'
alias root='sudo bash'
alias vimrc='vi ~/.vimrc'

alias vil='vi `ls -tr | tail -n 1`'

alias do='vi do ; cat do'

alias du1='du -h --max-depth=1'
alias du2='du -h --max-depth=2'

alias gdb1='gdb -ex "break main" -ex "run" -ex "set pagination off" '

# git clone git@github.com:swkim85/linux-drill.git

alias ga='git add'
alias gs='git status'
alias gcm='git commit -m "no message"'
alias gpp='git push'

alias go='cd ~/linux-drill/simd'
alias go1='cd /usr/lib/gcc/x86_64-linux-gnu/11/include'



