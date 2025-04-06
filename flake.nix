{
  description = "c_sandbox";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/release-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        name = "c_sandbox";
        src = ./.;
        pkgs = nixpkgs.legacyPackages.${system};
        # buildInputs = with pkgs; [clang-tools];
        nativeBuildInputs = with pkgs; [clang-tools];
      in {
        packages = {
          default = let
            inherit (pkgs) clangStdenv;
          in
            clangStdenv.mkDerivation {
              name = "c_sandbox";
              src = pkgs.lib.cleanSource ./.;
              # buildInputs = with pkgs; [];

              buildPhase = with pkgs; ''
                clang ./src/main.c -o c_sandbox
              '';

              installPhase = ''
                mkdir -p $out/bin
                cp c_sandbox $out/bin/c_sandbox
              '';
            };
        };

        devShells.default = pkgs.mkShell.override {stdenv = pkgs.clangStdenv;} {
          packages = with pkgs; [pkg-config clang-tools alejandra pre-commit];
          inputsFrom = [self.packages.${system}.default];
          # buildInputs = [
          #   pkgs.alejandra
          #   pkgs.gcc
          #   pkgs.gnumake
          #   pkgs.pkg-config
          #   pkgs.llvmPackages_19.clang-tools
          #   pkgs.pre-commit
          #   # pkgs.gdb
          #   # pkgs.valgrind
          # ];

          shellHook = with pkgs; ''
            # Source .bashrc
            . .bashrc
          '';
        };
      }
    );
}
