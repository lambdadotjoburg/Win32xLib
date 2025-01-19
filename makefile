.PHONY : dependencies
# see https://www.github.com/lambdadotjoburg/Win32xLib.git
# define variable to store github repo link
DEPENDENCIES_REPO := https://github.com/lambdadotjoburg/Win32xLib
# https://stackoverflow.com/questions/37353458/cloning-a-single-file-from-github-repo
# wget https://raw.githubusercontent.com/jquery/jquery/master/src/ajax.js
dependencies : ## recipe for setting up the `xLib` dependencies in the correct folder
> mkdir -p Project/dependencies;\
> git init;\
> git remote add origin $(DEPENDENCIES_REPO);\
> git config core.sparsecheckout true;\
> echo "Project/" >> .git/info/sparse-checkout;\
> git pull origin main;\
> rm -rf .git;\