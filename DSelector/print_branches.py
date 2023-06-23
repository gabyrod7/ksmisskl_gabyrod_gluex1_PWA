#!/d/home/gabyrod7/python/python-3.6.8/bin/python

import uproot

# Open the ROOT file
#file = uproot.open("snapshot.root")
file = uproot.open("ftree_dat_sp17.root")

# Access the desired TTree within the file
tree = file["kskl"]

# Get the list of branch names
branch_names = tree.keys()

# Print the branch names
for branch_name in branch_names:
    print('"'+branch_name+'"')

