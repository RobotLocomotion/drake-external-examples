#!/usr/bin/env groovy

/* SPDX-License-Identifier: MIT-0 */

def is_downstream_build = false
def current_build_causes = currentBuild.getBuildCauses()
for (cause in current_build_causes) {
  if (cause._class.toString().contains('UpstreamCause')) {
    is_downstream_build = true
  }
}

def props = []
props << parameters([
  string(name: 'drakeSha', defaultValue: 'master',
    description: 'Commit SHA or branch name. ' +
      'Enter the full commit SHA or branch name of Drake to test. ' +
      'Defaults to <code>master</code>.'),
])

if (env.BRANCH_NAME == 'main') {
  def triggers = []
  triggers << cron('H H(7-8) * * *')
  props << pipelineTriggers(triggers)
}

properties(props)

def examples = ['drake_bazel_external', 'drake_cmake_external']
def jobs = [:]

for (example in examples) {
  def name = example
  jobs[name] = {
    node('linux-noble-unprovisioned') {
      timeout(600) {
        ansiColor('xterm') {
          try {
            dir('src') {
              stage('checkout') {
                checkout scm
              }
            }
            dir('src/' + name) {
              stage(name + ' setup') {
                sh ".github/setup --drake-commit-hash ${params.drakeSha}"
              }
              stage(name + ' build and test') {
                sh ".github/ci_build_test --drake-commit-hash ${params.drakeSha}"
              }
            }
          } catch (e) {
            if (env.BRANCH_NAME == 'main' && !is_downstream_build) {
              emailext (
              subject: "Build failed in Jenkins: ${env.JOB_NAME} #${env.BUILD_NUMBER}",
              body: "See <${env.BUILD_URL}display/redirect?page=changes>",
              recipientProviders: [[$class: 'DevelopersRecipientProvider']],
              to: '$DEFAULT_RECIPIENTS',
              )
            }
            throw e
          }
        }
      }
    }
  }
}

parallel jobs
