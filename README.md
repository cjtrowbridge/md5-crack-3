# Sierra College: CSCI-46
This is a C program which cracks md5 passwords from a dictionary.

## Lab 10: Faster cracking

In this assignment you will be modifying the previous program to make it go faster by using a binary tree to search through its rainbow table.

## Prepare the Project
1. Log in to BitBucket and fork my crack3 (Links to an external site.)Links to an external site. repository.
1. Clone your repository. Use the git clone command.
1. Run make hashes to download the rockyou.txt file and build the md5.txt and passwords.txt files.
The rockyou.txt file contains the first three million entries from the original file.

## Make Changes
You need to change this file:

crack.c
Find the comments labeled TODO. These are things you need to do to complete the assignment.

Read the hashes file into an array of strings. (Same as the previous assignment.)
Read the dictionary into an array of strings. Each string should contain both the hash and the original word separated by some delimiter such as a space or colon. You will probably need two separate functions for reading the hashes and dictionaries since the work to be done for each is different.
Sort the dictionary array using qsort.
Search the array using bsearch and recover the plaintext password.

## What to Turn In
Commit your changes. Push your changes to BitBucket.

Do not add the rockyou.txt file to the repository! It is too large to handle effectively.

