echo "FROM BASHRC"

orange=$(tput setaf 166);
yellow=$(tput setaf 228);
green=$(tput setaf 71);
white=$(tput setaf 15);
bold=$(tput bold);
reset=$(tput sgr0);

PS1="\[${bold}\]\n";
PS1+="\[${orange}\]\u"; #username
PS1+="\[${white}\]@";
PS1+="\[${yellow}\]\h"; #host
PS1+="\[${white}\]:";
PS1+="\[${green}\]\W git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/'"; #working directory
PS1+="\n";
PS1+="\[${white}\]\$ \[${reset}\]";
export PS1;

parse_git_branch() {
    git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/'
}
