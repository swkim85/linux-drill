
# git clone git@github.com:swkim85/linux-drill.git
# sudo apt install net-tools gcc make gdb plocate


#   sudo apt install qemu-system-x86


#    sudo apt remove bash-completion
#    sudo vi /etc/bash_completion
#    sudo vi /etc/bash.bashrc

#   ln -s ~/linux-drill/bash_aliases ~/.bash_aliases
#   ln -s ~/linux-drill/vimrc  ~/.vimrc
#   source    ~/.bash_aliases
#

# git clone https://github.com/swkim85/linux-drill

alias vienv='vi ~/.bash_aliases ; source ~/.bash_aliases'
alias ll='ls -la'
alias lr='ls -ltr'

export PS1='\u@ SERVER  :\w\$ '
#export PS1='$ '
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'
alias root='sudo bash'
alias vimrc='vi ~/.vimrc'

alias vil='vi `ls -tr | tail -n 1`'

alias do1='vi do1 ; cat do1'
alias do2='vi do2 ; cat do2'
alias do3='vi do3 ; cat do3'
alias do4='vi do4 ; cat do4'

alias du1='du -h --max-depth=1'
alias du2='du -h --max-depth=2'

alias gdb1='gdb -ex "break main" -ex "run" -ex "set pagination off" '

alias ga='git add'
alias gs='git status'
alias gcm='git commit -m "no message"'
alias gpp='git push'

alias go='cd ~/linux-drill/rocm'
alias go1='cd /usr/lib/gcc/x86_64-linux-gnu/11/include'
alias go1='cd /opt/rocm-6.2.0/include/hip/'


export ROCM_PATH=/opt/rocm-6.2.0
export PATH=$PATH:$ROCM_PATH/bin:$ROCM_PATH/llvm/bin


