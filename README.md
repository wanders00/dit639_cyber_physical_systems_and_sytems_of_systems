# 2024-group-13

## Name <todo>

## Description
<insert text later>

## Installation & Usage
1. Clone the repository with SSH or HTTP.
```bash
git clone <URL of the repository>
```
2. Build the Docker image with the following command:
```bash
docker build -t CID/example:latest -f Dockerfile .
```
3. Run the program with the following command:
```bash
docker run --rm CID/example:latest 42
```

## Authors and acknowledgment
- William Andersson

- Andrii Demchenko

- Marko Mojsov

- Marcelo Santibanez

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Git Practices

### How to add new features
1. Create a new issue related to the feature you want to add, include labels and descriptions when appropriate.
2. Create a new branch with the issue number and title to address it.
3. Implement your changes.
4. Create a merge request to the main branch.
5. Assign the merge request to at least one other team member so they can review it.

### How to fix unexpected bugs 
1. Create a new issue related to the bug you want to fix and label it as bug.
2. Create a new branch with the issue number and title to address it.
3. Fix the bug and push the changes.
4. Create a merge request to the main branch.
5. Assign the merge request to at least one other team member so they can review it.

### Guidelines for commit messages 
`git commit "#<issue number> <type> <message>"`

1. Mention issue number at the start of the message.
2. [Optional] Add a type of the commit (e.g. fix, feature, docs, ci, etc.) in the commit message.
3. Write a message answering the following prompt: "This commit will ..."

Examples:
  ```
  git commit "#1 docs: add code of conduct"
  git commit "#2 docs: remove typo in README.md"
  git commit "#3 ci: change g++ version"
  ```
  ```
  git commit "#2 add a new section to README.md"
  git commit "#4 fix linting error in main.cpp"
  git commit "#5 improve complexity in sort.cpp"
  ```

