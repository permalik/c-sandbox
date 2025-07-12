# c-sandbox
sandbox:C Programming and Turnkey Ecosystem

### Development
Enter Nix shell
```sh
nix develop
```

#### Tooling
##### Format and Lint
Nix Format
```sh
alejandra <target>
```

C Format
```sh
c-format-all
```

C Lint
```sh
c-lint-all

c-lint-all-fix
```

Rebuild the environment
```sh
./build.sh
```

Repopulate and Rerun the Build
```sh
./run.sh
```
