$ git commit -m 'comment'

$ git branch branch1
$ git checkout branch1

$ git checkout -b branch2

$ git add .

$ git branch -d branch 2 	#löscht den branch2

# branch1 wird in den master gmerget
$ git checkout master
$ git merge branch1

$ git log

$ git hist

$ git status


#go back to commit
$ git checkout bab8c6fd306a1a4407ec537a0571574fa3597cc7

#set actual commit to head of a new branch
$ git checkout -b new_branch



#current branch to master
git checkout better_branch
git merge --strategy=ours --no-commit master     # keep the content of this branch, but record a merge
git commit -m "curBranch set as Master"       # add information to the template merge message
git checkout master
git merge better_branch             # fast-forward master up to the merge