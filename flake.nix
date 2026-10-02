{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    eachSystem = fn: nixpkgs.lib.genAttrs [
      "x86_64-linux"
      "aarch64-linux"
    ] (system: (fn {
      inherit system;
      pkgs = (import nixpkgs {
        inherit system;
      });
    }));
  in {
    packages = eachSystem ({ pkgs, ... }: {
      default = pkgs.stdenv.mkDerivation {
        pname = "nn";
        version = "0.1";

        src = ./.;

        nativeBuildInputs = with pkgs; [
          gcc
        ];

        buildPhase = ''
          make

          unity-test
        '';

        installPhase = ''
          mkdir -p $out/bin
          cp nn $out/bin/nn
        '';
      };
    });
    devShells = eachSystem ({ pkgs, ... }: {
      default = pkgs.mkShell {
        packages = with pkgs; [
          gcc
          gnumake

          clang-tools
          valgrind
          gdb

          unity-test
        ];
      };
    });
  };
}
