package org.kercoin.magrit.commands;

import java.io.IOException;
import java.util.Scanner;

import org.apache.sshd.server.Environment;
import org.eclipse.jgit.lib.ObjectId;
import org.eclipse.jgit.lib.Repository;
import org.kercoin.magrit.Context;
import org.kercoin.magrit.services.BuildQueueService;
import org.kercoin.magrit.services.UserIdentityService;
import org.kercoin.magrit.utils.UserIdentity;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.inject.Inject;
import com.google.inject.Singleton;

public class SendBuildCommand extends AbstractCommand<SendBuildCommand> {

	protected final Logger log = LoggerFactory.getLogger(getClass());

	@Singleton
	public static class SendBuildCommandProvider implements CommandProvider<SendBuildCommand> {

		private final Context ctx;
		private final BuildQueueService buildQueueService;
		private final UserIdentityService userService;
		
		@Inject
		public SendBuildCommandProvider(Context ctx, BuildQueueService buildQueueService, UserIdentityService userService) {
			this.ctx = ctx;
			this.buildQueueService = buildQueueService;
			this.userService = userService;
		}
		
		@Override
		public SendBuildCommand get() {
			return new SendBuildCommand(ctx, buildQueueService, userService);
		}

		@Override
		public boolean accept(String command) {
			return command.startsWith("magrit send-build ");
		}
		
	}
	
	private final BuildQueueService buildQueueService;
	private final UserIdentityService userService;
	
	private UserIdentity committer;
	
	private Repository repo;
	private String sha1;
	private boolean force;
	
	public SendBuildCommand(Context ctx, BuildQueueService buildQueueService, UserIdentityService userService) {
		super(ctx);
		this.buildQueueService = buildQueueService;
		this.userService = userService;
	}

	@Override
	public void run() {
		try {
			String userId = env.getEnv().get(Environment.ENV_USER);
			this.committer = userService.find(userId);
			sendBuild(repo.resolve(sha1));
			callback.onExit(0);
		} catch (Exception e) {
			e.printStackTrace();
			callback.onExit(1, e.getMessage());
		}
	}

	@Override
	public SendBuildCommand command(String command) throws IOException {
		// magrit build send SHA1
		// magrit build send --force SHA1
		Scanner scanner = new Scanner(command);
		scanner.useDelimiter("\\s{1,}");
		check(scanner.next(), "magrit");
		check(scanner.next(), "send-build");
		check(command, scanner.hasNext());
		String remainder = scanner.next();
		boolean force = false;
		if ("--force".equals(remainder)) {
			force = true;
			check(command, scanner.hasNext());
			remainder = scanner.next();
		}
		String repo = remainder;
		check(command, scanner.hasNext());
		String sha1 = scanner.next();
		checkSha1(sha1);
		
		this.force = force;
		this.sha1 = sha1;
		
		this.repo = createRepository(repo);
		
		return this;
	}

	boolean isForce() {
		return force;
	}
	
	void setForce(boolean force) {
		this.force = force;
	}
	
	String getSha1() {
		return sha1;
	}
	
	void setSha1(String sha1) {
		this.sha1 = sha1;
	}
	
	Repository getRepo() {
		return repo;
	}
	
	void setRepo(Repository repo) {
		this.repo = repo;
	}

	@Override
	protected Class<SendBuildCommand> getType() {
		return SendBuildCommand.class;
	}

	private void sendBuild(ObjectId newId) {
		try {
			if (buildQueueService.enqueueBuild(committer, repo, newId.getName(), force) != null) {
				String msg = String.format("Triggering build for commit %s on repository %s by %s.", newId.getName(), repo.getDirectory(), committer);
				log.info(msg);
				this.out.write('1');
			} else {
				String msg = String.format("Asked to build the commit %s on repository %s by %s but was skipped", newId.getName(), repo.getDirectory(), committer);
				log.info(msg);
				this.out.write('0');
			}
			this.out.flush();
		} catch (Exception e) {
			log.error("Unable to send build", e);
			e.printStackTrace();
		}
	}

}