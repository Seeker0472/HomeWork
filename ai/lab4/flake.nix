{
  description = "ysyx_workbench";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs-python.url = "github:cachix/nixpkgs-python";
  };
  ## Python 3.7
  ## Tensorflow 2.10
  ## Keras-tuner 1.4.7
  ## Matplotlib 3.8.0
  ## Protobuf 3.20.3

  outputs = { self, nixpkgs, flake-utils, nixpkgs-python }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config = { allowUnfree = true; };
        };

      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            nixpkgs-python.packages.x86_64-linux."3.9"
            libz
          ];

          NIX_LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
          ];

          LD_LIBRARY_PATH = "$LD_LIBRARY_PATH:${pkgs.stdenv.cc.cc.lib}/lib";

          nativeBuildInputs = with pkgs; [
            stdenv.cc.cc.lib
          ];

          NIX_LD = pkgs.lib.fileContents "${pkgs.stdenv.cc}/nix-support/dynamic-linker";
        };
      }
    );
}
