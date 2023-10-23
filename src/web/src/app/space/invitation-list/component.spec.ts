import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SpaceInvitationListComponent } from './component';

describe('SpaceInvitationListComponent', () => {
  let component: SpaceInvitationListComponent;
  let fixture: ComponentFixture<SpaceInvitationListComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ SpaceInvitationListComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SpaceInvitationListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
