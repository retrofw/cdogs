/*
    C-Dogs SDL
    A port of the legendary (and fun) action/arcade cdogs.
    Copyright (c) 2014-2015, Cong Xu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/
#include "handle_game_events.h"

#include "actor_placement.h"
#include "ai_utils.h"
#include "damage.h"
#include "game_events.h"
#include "net_server.h"
#include "objs.h"
#include "particle.h"
#include "pickup.h"
#include "triggers.h"

#define RELOAD_DISTANCE_PLUS 300

static void HandleGameEvent(
	const GameEvent e,
	Camera *camera,
	PowerupSpawner *healthSpawner,
	CArray *ammoSpawners);
void HandleGameEvents(
	CArray *store,
	Camera *camera,
	PowerupSpawner *healthSpawner,
	CArray *ammoSpawners)
{
	for (int i = 0; i < (int)store->size; i++)
	{
		GameEvent *e = CArrayGet(store, i);
		e->Delay--;
		if (e->Delay >= 0)
		{
			continue;
		}
		HandleGameEvent(*e, camera, healthSpawner, ammoSpawners);
	}
	GameEventsClear(store);
}
static void HandleGameEvent(
	const GameEvent e,
	Camera *camera,
	PowerupSpawner *healthSpawner,
	CArray *ammoSpawners)
{
	switch (e.Type)
	{
	case GAME_EVENT_PLAYER_DATA:
		PlayerDataAddOrUpdate(e.u.PlayerData);
		break;
	case GAME_EVENT_TILE_SET:
		{
			Tile *t = MapGetTile(&gMap, Net2Vec2i(e.u.TileSet.Pos));
			t->flags = e.u.TileSet.Flags;
			t->pic = PicManagerGetNamedPic(
				&gPicManager, e.u.TileSet.PicName);
			t->picAlt = PicManagerGetNamedPic(
				&gPicManager, e.u.TileSet.PicAltName);
		}
		break;
	case GAME_EVENT_MAP_OBJECT_ADD:
		ObjAdd(e.u.MapObjectAdd);
		break;
	case GAME_EVENT_MAP_OBJECT_DAMAGE:
		DamageObject(e.u.MapObjectDamage);
		break;
	case GAME_EVENT_SCORE:
		{
			PlayerData *p = PlayerDataGetByUID(e.u.Score.PlayerUID);
			PlayerScore(p, e.u.Score.Score);
			HUDAddUpdate(
				&camera->HUD,
				NUMBER_UPDATE_SCORE, e.u.Score.PlayerUID, e.u.Score.Score);
		}
		break;
	case GAME_EVENT_SOUND_AT:
		if (!e.u.SoundAt.IsHit || ConfigGetBool(&gConfig, "Sound.Hits"))
		{
			SoundPlayAt(
				&gSoundDevice,
				StrSound(e.u.SoundAt.Sound), Net2Vec2i(e.u.SoundAt.Pos));
		}
		break;
	case GAME_EVENT_SCREEN_SHAKE:
		camera->shake = ScreenShakeAdd(
			camera->shake, e.u.ShakeAmount,
			ConfigGetInt(&gConfig, "Graphics.ShakeMultiplier"));
		break;
	case GAME_EVENT_SET_MESSAGE:
		HUDDisplayMessage(
			&camera->HUD, e.u.SetMessage.Message, e.u.SetMessage.Ticks);
		break;
	case GAME_EVENT_GAME_START:
		gMission.HasStarted = true;
		break;
	case GAME_EVENT_ACTOR_ADD:
		ActorAdd(e.u.ActorAdd);
		break;
	case GAME_EVENT_ACTOR_MOVE:
		ActorMove(e.u.ActorMove);
		break;
	case GAME_EVENT_ACTOR_STATE:
		{
			TActor *a = ActorGetByUID(e.u.ActorState.UID);
			if (!a->isInUse) break;
			ActorSetState(a, (ActorAnimation)e.u.ActorState.State);
		}
		break;
	case GAME_EVENT_ACTOR_DIR:
		{
			TActor *a = ActorGetByUID(e.u.ActorDir.UID);
			if (!a->isInUse) break;
			a->direction = (direction_e)e.u.ActorDir.Dir;
		}
		break;
	case GAME_EVENT_ACTOR_SLIDE:
		{
			TActor *a = ActorGetByUID(e.u.ActorSlide.UID);
			if (!a->isInUse) break;
			a->Vel = Net2Vec2i(e.u.ActorSlide.Vel);
			// Slide sound
			if (ConfigGetBool(&gConfig, "Sound.Footsteps"))
			{
				SoundPlayAt(
					&gSoundDevice,
					gSoundDevice.slideSound,
					Vec2iNew(a->tileItem.x, a->tileItem.y));
			}
		}
		break;
	case GAME_EVENT_ACTOR_IMPULSE:
		{
			TActor *a = ActorGetByUID(e.u.ActorImpulse.UID);
			if (!a->isInUse) break;
			a->Vel = Vec2iAdd(a->Vel, Net2Vec2i(e.u.ActorImpulse.Vel));
			const Vec2i pos = Net2Vec2i(e.u.ActorImpulse.Pos);
			if (!Vec2iIsZero(pos))
			{
				a->Pos = pos;
			}
		}
		break;
	case GAME_EVENT_ACTOR_SWITCH_GUN:
		ActorSwitchGun(e.u.ActorSwitchGun);
		break;
	case GAME_EVENT_ACTOR_PICKUP_ALL:
		{
			TActor *a = ActorGetByUID(e.u.ActorPickupAll.UID);
			if (!a->isInUse) break;
			a->PickupAll = e.u.ActorPickupAll.PickupAll;
		}
		break;
	case GAME_EVENT_ACTOR_REPLACE_GUN:
		ActorReplaceGun(e.u.ActorReplaceGun);
		break;
	case GAME_EVENT_ACTOR_HEAL:
		{
			TActor *a = ActorGetByUID(e.u.Heal.UID);
			if (!a->isInUse || a->dead) break;
			ActorHeal(a, e.u.Heal.Amount);
			// Sound of healing
			SoundPlayAt(
				&gSoundDevice,
				gSoundDevice.healthSound, Vec2iFull2Real(a->Pos));
			// Tell the spawner that we took a health so we can
			// spawn more (but only if we're the server)
			if (e.u.Heal.IsRandomSpawned && !gCampaign.IsClient)
			{
				PowerupSpawnerRemoveOne(healthSpawner);
			}
			if (e.u.Heal.PlayerUID >= 0)
			{
				HUDAddUpdate(
					&camera->HUD, NUMBER_UPDATE_HEALTH,
					e.u.Heal.PlayerUID, e.u.Heal.Amount);
			}
		}
		break;
	case GAME_EVENT_ACTOR_ADD_AMMO:
		{
			TActor *a = ActorGetByUID(e.u.AddAmmo.UID);
			if (!a->isInUse || a->dead) break;
			ActorAddAmmo(a, e.u.AddAmmo.AmmoId, e.u.AddAmmo.Amount);
			// Tell the spawner that we took ammo so we can
			// spawn more (but only if we're the server)
			if (e.u.AddAmmo.IsRandomSpawned && !gCampaign.IsClient)
			{
				PowerupSpawnerRemoveOne(
					CArrayGet(ammoSpawners, e.u.AddAmmo.AmmoId));
			}
			if (e.u.AddAmmo.PlayerUID >= 0)
			{
				HUDAddUpdate(
					&camera->HUD, NUMBER_UPDATE_AMMO,
					e.u.AddAmmo.PlayerUID, e.u.AddAmmo.Amount);
			}
		}
		break;
	case GAME_EVENT_ACTOR_USE_AMMO:
		{
			TActor *a = ActorGetByUID(e.u.UseAmmo.UID);
			if (!a->isInUse || a->dead) break;
			ActorAddAmmo(a, e.u.UseAmmo.AmmoId, -(int)e.u.UseAmmo.Amount);
			if (e.u.UseAmmo.PlayerUID >= 0)
			{
				HUDAddUpdate(
					&camera->HUD, NUMBER_UPDATE_AMMO,
					e.u.UseAmmo.PlayerUID, -(int)e.u.UseAmmo.Amount);
			}
		}
		break;
	case GAME_EVENT_ACTOR_DIE:
		{
			TActor *a = ActorGetByUID(e.u.ActorDie.UID);

			// Check if the player has lives to revive
			PlayerData *p = PlayerDataGetByUID(a->PlayerUID);
			if (p != NULL)
			{
				p->Lives--;
				CASSERT(p->Lives >= 0, "Player has died too many times");
				if (p->Lives > 0 && !gCampaign.IsClient)
				{
					// Find the closest player alive; try to spawn next to that position
					// if no other suitable position exists
					Vec2i defaultSpawnPosition = Vec2iZero();
					const TActor *closestActor = AIGetClosestPlayer(a->Pos);
					if (closestActor != NULL) defaultSpawnPosition = closestActor->Pos;
					PlacePlayer(&gMap, p, defaultSpawnPosition, false);
				}
			}

			ActorDestroy(a);
		}
		break;
	case GAME_EVENT_ACTOR_MELEE:
		{
			const TActor *a = ActorGetByUID(e.u.Melee.UID);
			if (!a->isInUse) break;
			const BulletClass *b = StrBulletClass(e.u.Melee.BulletClass);
			if ((HitType)e.u.Melee.HitType != HIT_NONE &&
				HasHitSound(b->Power, a->flags, a->PlayerUID,
				(TileItemKind)e.u.Melee.TargetKind, e.u.Melee.TargetUID,
				SPECIAL_NONE, false))
			{
				PlayHitSound(
					&b->HitSound, (HitType)e.u.Melee.HitType,
					Vec2iFull2Real(a->Pos));
			}
			if (!gCampaign.IsClient)
			{
				Damage(
					Vec2iZero(),
					b->Power,
					a->flags, a->PlayerUID, a->uid,
					(TileItemKind)e.u.Melee.TargetKind, e.u.Melee.TargetUID,
					SPECIAL_NONE);
			}
		}
		break;
	case GAME_EVENT_ADD_PICKUP:
		PickupAdd(e.u.AddPickup);
		// Play a spawn sound
		SoundPlayAt(
			&gSoundDevice,
			StrSound("spawn_item"), Net2Vec2i(e.u.AddPickup.Pos));
		break;
	case GAME_EVENT_REMOVE_PICKUP:
		PickupDestroy(e.u.RemovePickup.UID);
		if (e.u.RemovePickup.SpawnerUID >= 0)
		{
			TObject *o = ObjGetByUID(e.u.RemovePickup.SpawnerUID);
			o->counter = AMMO_SPAWNER_RESPAWN_TICKS;
		}
		break;
	case GAME_EVENT_BULLET_BOUNCE:
		{
			TMobileObject *o = MobObjGetByUID(e.u.BulletBounce.UID);
			if (o == NULL || !o->isInUse) break;
			const Vec2i pos = Net2Vec2i(e.u.BulletBounce.BouncePos);
			PlayHitSound(
				&o->bulletClass->HitSound, (HitType)e.u.BulletBounce.HitType,
				Vec2iFull2Real(pos));
			if (e.u.BulletBounce.Spark && o->bulletClass->Spark != NULL)
			{
				GameEvent s = GameEventNew(GAME_EVENT_ADD_PARTICLE);
				s.u.AddParticle.Class = o->bulletClass->Spark;
				s.u.AddParticle.FullPos = pos;
				s.u.AddParticle.Z = o->z;
				GameEventsEnqueue(&gGameEvents, s);
			}
			o->x = pos.x;
			o->y = pos.y;
			o->vel = Net2Vec2i(e.u.BulletBounce.BounceVel);
		}
		break;
	case GAME_EVENT_REMOVE_BULLET:
		{
			TMobileObject *o = MobObjGetByUID(e.u.RemoveBullet.UID);
			if (o == NULL || !o->isInUse) break;
			MobObjDestroy(o);
		}
		break;
	case GAME_EVENT_PARTICLE_REMOVE:
		ParticleDestroy(&gParticles, e.u.ParticleRemoveId);
		break;
	case GAME_EVENT_GUN_FIRE:
		{
			const GunDescription *g = StrGunDescription(e.u.GunFire.Gun);
			const Vec2i fullPos = Net2Vec2i(e.u.GunFire.MuzzleFullPos);

			// Add bullets
			if (g->Bullet && !gCampaign.IsClient)
			{
				// Find the starting angle of the spread (clockwise)
				// Keep in mind the fencepost problem, i.e. spread of 3 means a
				// total spread angle of 2x width
				const double spreadStartAngle =
					g->AngleOffset -
					(g->Spread.Count - 1) * g->Spread.Width / 2;
				for (int i = 0; i < g->Spread.Count; i++)
				{
					const double recoil =
						((double)rand() / RAND_MAX * g->Recoil) -
						g->Recoil / 2;
					const double finalAngle =
						e.u.GunFire.Angle + spreadStartAngle +
						i * g->Spread.Width + recoil;
					GameEvent ab = GameEventNew(GAME_EVENT_ADD_BULLET);
					ab.u.AddBullet.UID = MobObjsObjsGetNextUID();
					strcpy(ab.u.AddBullet.BulletClass, g->Bullet->Name);
					ab.u.AddBullet.MuzzlePos = Vec2i2Net(fullPos);
					ab.u.AddBullet.MuzzleHeight = e.u.GunFire.Z;
					ab.u.AddBullet.Angle = (float)finalAngle;
					ab.u.AddBullet.Elevation =
						RAND_INT(g->ElevationLow, g->ElevationHigh);
					ab.u.AddBullet.Flags = e.u.GunFire.Flags;
					ab.u.AddBullet.PlayerUID = e.u.GunFire.PlayerUID;
					ab.u.AddBullet.ActorUID = e.u.GunFire.UID;
					GameEventsEnqueue(&gGameEvents, ab);
				}
			}

			// Add muzzle flash
			if (GunHasMuzzle(g))
			{
				GameEvent ap = GameEventNew(GAME_EVENT_ADD_PARTICLE);
				ap.u.AddParticle.Class = g->MuzzleFlash;
				ap.u.AddParticle.FullPos = fullPos;
				ap.u.AddParticle.Z = e.u.GunFire.Z;
				ap.u.AddParticle.Angle = e.u.GunFire.Angle;
				GameEventsEnqueue(&gGameEvents, ap);
			}
			// Sound
			if (e.u.GunFire.Sound && g->Sound)
			{
				SoundPlayAt(&gSoundDevice, g->Sound, Vec2iFull2Real(fullPos));
			}
			// Screen shake
			if (g->ShakeAmount > 0)
			{
				GameEvent s = GameEventNew(GAME_EVENT_SCREEN_SHAKE);
				s.u.ShakeAmount = g->ShakeAmount;
				GameEventsEnqueue(&gGameEvents, s);
			}
			// Brass shells
			// If we have a reload lead, defer the creation of shells until then
			if (g->Brass && g->ReloadLead == 0)
			{
				const direction_e d = RadiansToDirection(e.u.GunFire.Angle);
				const Vec2i muzzleOffset = GunGetMuzzleOffset(g, d);
				GunAddBrass(g, d, Vec2iMinus(fullPos, muzzleOffset));
			}
		}
		break;
	case GAME_EVENT_GUN_RELOAD:
		{
			const GunDescription *g = StrGunDescription(e.u.GunReload.Gun);
			const Vec2i fullPos = Net2Vec2i(e.u.GunReload.FullPos);
			SoundPlayAtPlusDistance(
				&gSoundDevice,
				g->ReloadSound,
				Vec2iFull2Real(fullPos),
				RELOAD_DISTANCE_PLUS);
			// Brass shells
			if (g->Brass)
			{
				GunAddBrass(g, (direction_e)e.u.GunReload.Direction, fullPos);
			}
		}
		break;
	case GAME_EVENT_GUN_STATE:
		{
			const TActor *a = ActorGetByUID(e.u.GunState.ActorUID);
			if (!a->isInUse) break;
			WeaponSetState(ActorGetGun(a), (gunstate_e)e.u.GunState.State);
		}
		break;
	case GAME_EVENT_ADD_BULLET:
		BulletAdd(e.u.AddBullet);
		break;
	case GAME_EVENT_ADD_PARTICLE:
		ParticleAdd(&gParticles, e.u.AddParticle);
		break;
	case GAME_EVENT_ACTOR_HIT:
		{
			TActor *a = ActorGetByUID(e.u.ActorHit.UID);
			if (!a->isInUse) break;
			ActorTakeHit(a, e.u.ActorHit.Special);
			if (e.u.ActorHit.Power > 0)
			{
				DamageActor(
					a, e.u.ActorHit.Power, e.u.ActorHit.HitterPlayerUID);
				if (e.u.ActorHit.PlayerUID >= 0)
				{
					HUDAddUpdate(
						&camera->HUD, NUMBER_UPDATE_HEALTH,
						e.u.ActorHit.PlayerUID, -e.u.ActorHit.Power);
				}

				AddBloodSplatter(
					a->Pos, e.u.ActorHit.Power,
					Net2Vec2i(e.u.ActorHit.Vel));
			}
		}
		break;
	case GAME_EVENT_TRIGGER:
		{
			const Tile *t =
				MapGetTile(&gMap, Net2Vec2i(e.u.TriggerEvent.Tile));
			CA_FOREACH(Trigger *, tp, t->triggers)
				if ((*tp)->id == (int)e.u.TriggerEvent.ID)
				{
					TriggerActivate(*tp, &gMap.triggers);
					break;
				}
			CA_FOREACH_END()
		}
		break;
	case GAME_EVENT_EXPLORE_TILES:
		// Process runs of explored tiles
		for (int i = 0; i < (int)e.u.ExploreTiles.Runs_count; i++)
		{
			Vec2i tile = Net2Vec2i(e.u.ExploreTiles.Runs[i].Tile);
			for (int j = 0; j < e.u.ExploreTiles.Runs[i].Run; j++)
			{
				MapMarkAsVisited(&gMap, tile);
				tile.x++;
				if (tile.x == gMap.Size.x)
				{
					tile.x = 0;
					tile.y++;
				}
			}
		}
		break;
	case GAME_EVENT_RESCUE_CHARACTER:
		{
			TActor *a = ActorGetByUID(e.u.Rescue.UID);
			if (!a->isInUse) break;
			a->flags &= ~FLAGS_PRISONER;
			SoundPlayAt(
				&gSoundDevice, StrSound("rescue"), Vec2iFull2Real(a->Pos));
		}
		break;
	case GAME_EVENT_OBJECTIVE_UPDATE:
		{
			ObjectiveDef *o = CArrayGet(
				&gMission.Objectives, e.u.ObjectiveUpdate.ObjectiveId);
			o->done += e.u.ObjectiveUpdate.Count;
			// Display a text update effect for the objective
			HUDAddUpdate(
				&camera->HUD, NUMBER_UPDATE_OBJECTIVE,
				e.u.ObjectiveUpdate.ObjectiveId, e.u.ObjectiveUpdate.Count);
			MissionSetMessageIfComplete(&gMission);
		}
		break;
	case GAME_EVENT_ADD_KEYS:
		gMission.KeyFlags |= e.u.AddKeys.KeyFlags;
		SoundPlayAt(
			&gSoundDevice, gSoundDevice.keySound, Net2Vec2i(e.u.AddKeys.Pos));
		// Clear cache since we may now have new paths
		PathCacheClear(&gPathCache);
		break;
	case GAME_EVENT_MISSION_COMPLETE:
		if (e.u.MissionComplete.ShowMsg)
		{
			HUDDisplayMessage(&camera->HUD, "Mission complete", -1);
		}
		camera->HUD.showExit = true;
		MapShowExitArea(&gMap);
		break;
	case GAME_EVENT_MISSION_INCOMPLETE:
		gMission.state = MISSION_STATE_PLAY;
		break;
	case GAME_EVENT_MISSION_PICKUP:
		gMission.state = MISSION_STATE_PICKUP;
		gMission.pickupTime = gMission.time;
		SoundPlay(&gSoundDevice, StrSound("whistle"));
		break;
	case GAME_EVENT_MISSION_END:
		gMission.isDone = true;
		break;
	default:
		assert(0 && "unknown game event");
		break;
	}
}
